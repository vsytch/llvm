add_llvm_loadable_module( LLVMSugarAddrSpaceCast
  SugarAddrSpaceCast.cpp
  )

add_dependencies(LLVMSugarAddrSpaceCast intrinsics_gen)
