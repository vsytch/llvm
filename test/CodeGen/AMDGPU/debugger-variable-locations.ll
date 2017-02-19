; RUN: llc -O0 -mtriple=amdgcn--amdhsa -mcpu=fiji -verify-machineinstrs -filetype=obj < %s | llvm-dwarfdump -debug-dump=info - | FileCheck %s
declare void @llvm.dbg.declare(metadata, metadata, metadata)

@GlobA = common addrspace(1) global i32 addrspace(1)* null, align 4, !dbg !0
@GlobB = common addrspace(1) global i32 addrspace(1)* null, align 4, !dbg !6

; CHECK: DW_AT_name [DW_FORM_strp] ( .debug_str[0x00000025] = "GlobA")
; CHECK: DW_AT_location [DW_FORM_block1] (<0x09> 03 00 00 00 00 10 01 16 18 )
; CHECK: DW_AT_name [DW_FORM_strp] ( .debug_str[0x0000002f] = "GlobB")
; CHECK: DW_AT_location [DW_FORM_block1] (<0x09> 03 00 00 00 00 10 01 16 18 )
; CHECK: DW_AT_location [DW_FORM_block1] (<0x06> 91 00 10 00 16 18 )
; CHECK: DW_AT_name [DW_FORM_strp] ( .debug_str[0x0000003a] = "A")
; CHECK: DW_AT_location [DW_FORM_block1] (<0x06> 91 08 10 00 16 18 )
; CHECK: DW_AT_name [DW_FORM_strp] ( .debug_str[0x0000003c] = "B")

define amdgpu_kernel void @test(
    i32 addrspace(1)* %A,
    i32 addrspace(1)* %B) !dbg !15 {
entry:
  %A.addr = alloca i32 addrspace(1)*, align 4
  %B.addr = alloca i32 addrspace(1)*, align 4
  store i32 addrspace(1)* %A, i32 addrspace(1)** %A.addr, align 4
  call void @llvm.dbg.declare(metadata i32 addrspace(1)** %A.addr, metadata !22, metadata !23), !dbg !24
  store i32 addrspace(1)* %B, i32 addrspace(1)** %B.addr, align 4
  call void @llvm.dbg.declare(metadata i32 addrspace(1)** %B.addr, metadata !25, metadata !23), !dbg !26
  %0 = load i32 addrspace(1)*, i32 addrspace(1)* addrspace(1)* @GlobA, align 4, !dbg !27
  %1 = load i32, i32 addrspace(1)* %0, align 4, !dbg !28
  %2 = load i32 addrspace(1)*, i32 addrspace(1)** %A.addr, align 4, !dbg !29
  store i32 %1, i32 addrspace(1)* %2, align 4, !dbg !30
  %3 = load i32 addrspace(1)*, i32 addrspace(1)* addrspace(1)* @GlobB, align 4, !dbg !31
  %4 = load i32, i32 addrspace(1)* %3, align 4, !dbg !32
  %5 = load i32 addrspace(1)*, i32 addrspace(1)** %B.addr, align 4, !dbg !33
  store i32 %4, i32 addrspace(1)* %5, align 4, !dbg !34
  ret void, !dbg !35
}

!llvm.dbg.cu = !{!2}
!opencl.ocl.version = !{!11}
!llvm.module.flags = !{!12, !13}
!llvm.ident = !{!14}

!0 = !DIGlobalVariableExpression(var: !1, expr: !10)
!1 = distinct !DIGlobalVariable(name: "GlobA", scope: !2, file: !3, line: 2, type: !8, isLocal: false, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C99, file: !3, producer: "", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !4, globals: !5)
!3 = !DIFile(filename: "test.cl", directory: "/home/kzhuravl/Compute/test")
!4 = !{}
!5 = !{!0, !6}
!6 = !DIGlobalVariableExpression(var: !7, expr: !10)
!7 = distinct !DIGlobalVariable(name: "GlobB", scope: !2, file: !3, line: 3, type: !8, isLocal: false, isDefinition: true)
!8 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !9, size: 64, addressSpace: 1)
!9 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!10 = !DIExpression(DW_OP_constu, 1, DW_OP_swap, DW_OP_xderef)
!11 = !{i32 2, i32 0}
!12 = !{i32 2, !"Dwarf Version", i32 2}
!13 = !{i32 2, !"Debug Info Version", i32 3}
!14 = !{!""}
!15 = distinct !DISubprogram(name: "test", scope: !3, file: !3, line: 5, type: !16, isLocal: false, isDefinition: true, scopeLine: 5, flags: DIFlagPrototyped, isOptimized: false, unit: !2, variables: !4)
!16 = !DISubroutineType(types: !17)
!17 = !{null, !8, !8}
!18 = !{i32 1, i32 1}
!19 = !{!"none", !"none"}
!20 = !{!"int*", !"int*"}
!21 = !{!"", !""}
!22 = !DILocalVariable(name: "A", arg: 1, scope: !15, file: !3, line: 5, type: !8)
!23 = !DIExpression(DW_OP_constu, 0, DW_OP_swap, DW_OP_xderef)
!24 = !DILocation(line: 5, column: 33, scope: !15)
!25 = !DILocalVariable(name: "B", arg: 2, scope: !15, file: !3, line: 5, type: !8)
!26 = !DILocation(line: 5, column: 48, scope: !15)
!27 = !DILocation(line: 6, column: 9, scope: !15)
!28 = !DILocation(line: 6, column: 8, scope: !15)
!29 = !DILocation(line: 6, column: 4, scope: !15)
!30 = !DILocation(line: 6, column: 6, scope: !15)
!31 = !DILocation(line: 7, column: 9, scope: !15)
!32 = !DILocation(line: 7, column: 8, scope: !15)
!33 = !DILocation(line: 7, column: 4, scope: !15)
!34 = !DILocation(line: 7, column: 6, scope: !15)
!35 = !DILocation(line: 8, column: 1, scope: !15)
