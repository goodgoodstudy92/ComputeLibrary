/*
 * Copyright (c) 2016-2020 ARM Limited.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "arm_compute/core/CL/kernels/CLGaussian3x3Kernel.h"

#include "arm_compute/core/CL/CLKernelLibrary.h"
#include "arm_compute/core/CL/ICLTensor.h"
#include "arm_compute/core/CL/OpenCL.h"
#include "arm_compute/core/Types.h"
#include "arm_compute/core/Validate.h"

#include <set>
#include <string>

using namespace arm_compute;

BorderSize CLGaussian3x3Kernel::border_size() const
{
    return BorderSize(1);
}

void CLGaussian3x3Kernel::configure(const ICLTensor *input, ICLTensor *output, bool border_undefined)
{
    configure(CLKernelLibrary::get().get_compile_context(), input, output, border_undefined);
}

void CLGaussian3x3Kernel::configure(const CLCompileContext &compile_context, const ICLTensor *input, ICLTensor *output, bool border_undefined)
{
    ARM_COMPUTE_ERROR_ON_DATA_TYPE_CHANNEL_NOT_IN(input, 1, DataType::U8);
    ARM_COMPUTE_ERROR_ON_DATA_TYPE_CHANNEL_NOT_IN(output, 1, DataType::U8);

    _input  = input;
    _output = output;

    // Set build options
    std::set<std::string> build_opts = { "-DMAT0=1", "-DMAT1=2", "-DMAT2=1",
                                         "-DMAT3=2", "-DMAT4=4", "-DMAT5=2",
                                         "-DMAT6=1", "-DMAT7=2", "-DMAT8=1",
                                         "-DSCALE=16", "-DDATA_TYPE_OUT=uchar"
                                       };

    // Create kernel
    _kernel = create_kernel(compile_context, "convolution3x3_static", build_opts);

    // Configure kernel window
    constexpr unsigned int num_elems_processed_per_iteration = 8;
    constexpr unsigned int num_elems_written_per_iteration   = 8;
    constexpr unsigned int num_elems_read_per_iteration      = 16;
    constexpr unsigned int num_rows_read_per_iteration       = 3;

    Window win = calculate_max_window(*input->info(), Steps(num_elems_processed_per_iteration), border_undefined, border_size());

    AccessWindowRectangle  input_access(input->info(), -border_size().left, -border_size().top, num_elems_read_per_iteration, num_rows_read_per_iteration);
    AccessWindowHorizontal output_access(output->info(), 0, num_elems_written_per_iteration);

    update_window_and_padding(win, input_access, output_access);

    output_access.set_valid_region(win, input->info()->valid_region(), border_undefined, border_size());

    ICLKernel::configure_internal(win);
}
