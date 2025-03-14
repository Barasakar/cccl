// This file was automatically generated. Do not edit.

#ifndef _CUDA_PTX_GENERATED_CP_ASYNC_BULK_COMMIT_GROUP_H_
#define _CUDA_PTX_GENERATED_CP_ASYNC_BULK_COMMIT_GROUP_H_

/*
// cp.async.bulk.commit_group; // PTX ISA 80, SM_90
template <typename = void>
__device__ static inline void cp_async_bulk_commit_group();
*/
#if __cccl_ptx_isa >= 800
extern "C" _CCCL_DEVICE void __cuda_ptx_cp_async_bulk_commit_group_is_not_supported_before_SM_90__();
template <typename = void>
_CCCL_DEVICE static inline void cp_async_bulk_commit_group()
{
#  if _CCCL_CUDA_COMPILER(NVHPC) || __CUDA_ARCH__ >= 900
  asm volatile("cp.async.bulk.commit_group;" : : :);
#  else
  // Unsupported architectures will have a linker error with a semi-decent error message
  __cuda_ptx_cp_async_bulk_commit_group_is_not_supported_before_SM_90__();
#  endif
}
#endif // __cccl_ptx_isa >= 800

#endif // _CUDA_PTX_GENERATED_CP_ASYNC_BULK_COMMIT_GROUP_H_
