/*
 * Copyright (c) 2017-2020 ARM Limited.
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
#ifndef ARM_COMPUTE_CLRESHAPELAYERKERNEL_H
#define ARM_COMPUTE_CLRESHAPELAYERKERNEL_H

#include "arm_compute/core/CL/ICLKernel.h"
#include "arm_compute/core/Types.h"

namespace arm_compute
{
class ICLTensor;

/** Interface for the kernel to perform tensor reshaping */
class CLReshapeLayerKernel : public ICLKernel
{
public:
    /** Default constructor */
    CLReshapeLayerKernel();
    /** Prevent instances of this class from being copied (As this class contains pointers) */
    CLReshapeLayerKernel(const CLReshapeLayerKernel &) = delete;
    /** Prevent instances of this class from being copied (As this class contains pointers) */
    CLReshapeLayerKernel &operator=(const CLReshapeLayerKernel &) = delete;
    /** Allow instances of this class to be moved */
    CLReshapeLayerKernel(CLReshapeLayerKernel &&) = default;
    /** Allow instances of this class to be moved */
    CLReshapeLayerKernel &operator=(CLReshapeLayerKernel &&) = default;
    /** Default destructor */
    ~CLReshapeLayerKernel() = default;
    /** Set the input and output of the kernel
     *
     * @param[in]  input  Source tensor. Data type supported: All.
     * @param[out] output Destination tensor. Data type supported: Same as @p input
     */
    void configure(const ICLTensor *input, ICLTensor *output);
    /** Set the input and output of the kernel
     *
     * @param[in]  compile_context The compile context to be used.
     * @param[in]  input           Source tensor. Data type supported: All.
     * @param[out] output          Destination tensor. Data type supported: Same as @p input
     */
    void configure(const CLCompileContext &compile_context, const ICLTensor *input, ICLTensor *output);

    /** Static function to check if given info will lead to a valid configuration of @ref CLReshapeLayerKernel
     *
     * @param[in] input  Source tensor info. Data type supported: All
     * @param[in] output Destination tensor info. Data type supported: Same as @p input
     *
     * @return a status
     */
    static Status validate(const ITensorInfo *input, const ITensorInfo *output);

    // Inherited methods overridden:
    void run(const Window &window, cl::CommandQueue &queue) override;

private:
    const ICLTensor *_input;  /**< Source tensor */
    ICLTensor       *_output; /**< Destination tensor */
};
} // namespace arm_compute
#endif /*ARM_COMPUTE_CLRESHAPELAYERKERNEL_H */
