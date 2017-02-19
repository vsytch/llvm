; RUN: llc -O0 -mtriple=amdgcn--amdhsa -mcpu=fiji -verify-machineinstrs < %s | FileCheck %s
declare void @llvm.dbg.declare(metadata, metadata, metadata)

@test.Var4 = internal addrspace(3) global i32 addrspace(1)* undef, align 4, !dbg !0
@test.Var5 = internal addrspace(3) global i32 addrspace(2)* undef, align 4, !dbg !13
@test.Var6 = internal addrspace(3) global i32 addrspace(3)* undef, align 4, !dbg !15
@test.Var7 = internal addrspace(3) global i32* undef, align 4, !dbg !17

; CHECK-DAG: .long 0 ; DW_AT_address_class
; CHECK-DAG: .long 1 ; DW_AT_address_class
; CHECK-DAG: .long 2 ; DW_AT_address_class
; CHECK-DAG: .long 3 ; DW_AT_address_class

define amdgpu_kernel void @test(
    i32 addrspace(1)* %Arg0,
    i32 addrspace(2)* %Arg1,
    i32 addrspace(3)* %Arg2) !dbg !2 {
entry:
  %Arg0.addr = alloca i32 addrspace(1)*, align 4
  %Arg1.addr = alloca i32 addrspace(2)*, align 4
  %Arg2.addr = alloca i32 addrspace(3)*, align 4
  %Tmp = alloca i32*, align 4
  %Var0 = alloca i32 addrspace(1)*, align 4
  %Var1 = alloca i32 addrspace(2)*, align 4
  %Var2 = alloca i32 addrspace(3)*, align 4
  %Var3 = alloca i32*, align 4
  %Var8 = alloca i32 addrspace(1)*, align 4
  %Var9 = alloca i32 addrspace(2)*, align 4
  %Var10 = alloca i32 addrspace(3)*, align 4
  %Var11 = alloca i32*, align 4
  store i32 addrspace(1)* %Arg0, i32 addrspace(1)** %Arg0.addr, align 4
  call void @llvm.dbg.declare(metadata i32 addrspace(1)** %Arg0.addr, metadata !28, metadata !29), !dbg !30
  store i32 addrspace(2)* %Arg1, i32 addrspace(2)** %Arg1.addr, align 4
  call void @llvm.dbg.declare(metadata i32 addrspace(2)** %Arg1.addr, metadata !31, metadata !29), !dbg !32
  store i32 addrspace(3)* %Arg2, i32 addrspace(3)** %Arg2.addr, align 4
  call void @llvm.dbg.declare(metadata i32 addrspace(3)** %Arg2.addr, metadata !33, metadata !29), !dbg !34
  call void @llvm.dbg.declare(metadata i32** %Tmp, metadata !35, metadata !29), !dbg !36
  call void @llvm.dbg.declare(metadata i32 addrspace(1)** %Var0, metadata !37, metadata !29), !dbg !38
  %0 = load i32 addrspace(1)*, i32 addrspace(1)** %Arg0.addr, align 4, !dbg !39
  store i32 addrspace(1)* %0, i32 addrspace(1)** %Var0, align 4, !dbg !38
  call void @llvm.dbg.declare(metadata i32 addrspace(2)** %Var1, metadata !40, metadata !29), !dbg !41
  %1 = load i32 addrspace(2)*, i32 addrspace(2)** %Arg1.addr, align 4, !dbg !42
  store i32 addrspace(2)* %1, i32 addrspace(2)** %Var1, align 4, !dbg !41
  call void @llvm.dbg.declare(metadata i32 addrspace(3)** %Var2, metadata !43, metadata !29), !dbg !44
  %2 = load i32 addrspace(3)*, i32 addrspace(3)** %Arg2.addr, align 4, !dbg !45
  store i32 addrspace(3)* %2, i32 addrspace(3)** %Var2, align 4, !dbg !44
  call void @llvm.dbg.declare(metadata i32** %Var3, metadata !46, metadata !29), !dbg !47
  %3 = load i32*, i32** %Tmp, align 4, !dbg !48
  store i32* %3, i32** %Var3, align 4, !dbg !47
  %4 = load i32 addrspace(1)*, i32 addrspace(1)** %Arg0.addr, align 4, !dbg !49
  store i32 addrspace(1)* %4, i32 addrspace(1)* addrspace(3)* @test.Var4, align 4, !dbg !50
  %5 = load i32 addrspace(2)*, i32 addrspace(2)** %Arg1.addr, align 4, !dbg !51
  store i32 addrspace(2)* %5, i32 addrspace(2)* addrspace(3)* @test.Var5, align 4, !dbg !52
  %6 = load i32 addrspace(3)*, i32 addrspace(3)** %Arg2.addr, align 4, !dbg !53
  store i32 addrspace(3)* %6, i32 addrspace(3)* addrspace(3)* @test.Var6, align 4, !dbg !54
  %7 = load i32*, i32** %Tmp, align 4, !dbg !55
  store i32* %7, i32* addrspace(3)* @test.Var7, align 4, !dbg !56
  call void @llvm.dbg.declare(metadata i32 addrspace(1)** %Var8, metadata !57, metadata !29), !dbg !58
  call void @llvm.dbg.declare(metadata i32 addrspace(2)** %Var9, metadata !59, metadata !29), !dbg !60
  call void @llvm.dbg.declare(metadata i32 addrspace(3)** %Var10, metadata !61, metadata !29), !dbg !62
  call void @llvm.dbg.declare(metadata i32** %Var11, metadata !63, metadata !29), !dbg !64
  ret void, !dbg !65
}

!llvm.dbg.cu = !{!10}
!opencl.ocl.version = !{!20}
!llvm.module.flags = !{!21, !22}
!llvm.ident = !{!23}

!0 = !DIGlobalVariableExpression(var: !1)
!1 = distinct !DIGlobalVariable(name: "Var4", scope: !2, file: !3, line: 9, type: !6, isLocal: true, isDefinition: true)
!2 = distinct !DISubprogram(name: "test", scope: !3, file: !3, line: 1, type: !4, isLocal: false, isDefinition: true, scopeLine: 1, flags: DIFlagPrototyped, isOptimized: false, unit: !10, variables: !11)
!3 = !DIFile(filename: "test.cl", directory: "/home/kzhuravl/Compute/test")
!4 = !DISubroutineType(types: !5)
!5 = !{null, !6, !8, !9}
!6 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !7, size: 64, addressSpace: 1)
!7 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!8 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !7, size: 64, addressSpace: 2)
!9 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !7, size: 32, addressSpace: 3)
!10 = distinct !DICompileUnit(language: DW_LANG_C99, file: !3, producer: "", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !11, globals: !12)
!11 = !{}
!12 = !{!0, !13, !15, !17}
!13 = !DIGlobalVariableExpression(var: !14)
!14 = distinct !DIGlobalVariable(name: "Var5", scope: !2, file: !3, line: 10, type: !8, isLocal: true, isDefinition: true)
!15 = !DIGlobalVariableExpression(var: !16)
!16 = distinct !DIGlobalVariable(name: "Var6", scope: !2, file: !3, line: 11, type: !9, isLocal: true, isDefinition: true)
!17 = !DIGlobalVariableExpression(var: !18)
!18 = distinct !DIGlobalVariable(name: "Var7", scope: !2, file: !3, line: 12, type: !19, isLocal: true, isDefinition: true)
!19 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !7, size: 32)
!20 = !{i32 2, i32 0}
!21 = !{i32 2, !"Dwarf Version", i32 2}
!22 = !{i32 2, !"Debug Info Version", i32 3}
!23 = !{!""}
!24 = !{i32 1, i32 2, i32 3}
!25 = !{!"none", !"none", !"none"}
!26 = !{!"int*", !"int*", !"int*"}
!27 = !{!"", !"const", !""}
!28 = !DILocalVariable(name: "Arg0", arg: 1, scope: !2, file: !3, line: 1, type: !6)
!29 = !DIExpression()
!30 = !DILocation(line: 1, column: 30, scope: !2)
!31 = !DILocalVariable(name: "Arg1", arg: 2, scope: !2, file: !3, line: 1, type: !8)
!32 = !DILocation(line: 1, column: 50, scope: !2)
!33 = !DILocalVariable(name: "Arg2", arg: 3, scope: !2, file: !3, line: 1, type: !9)
!34 = !DILocation(line: 1, column: 67, scope: !2)
!35 = !DILocalVariable(name: "Tmp", scope: !2, file: !3, line: 2, type: !19)
!36 = !DILocation(line: 2, column: 16, scope: !2)
!37 = !DILocalVariable(name: "Var0", scope: !2, file: !3, line: 4, type: !6)
!38 = !DILocation(line: 4, column: 24, scope: !2)
!39 = !DILocation(line: 4, column: 31, scope: !2)
!40 = !DILocalVariable(name: "Var1", scope: !2, file: !3, line: 5, type: !8)
!41 = !DILocation(line: 5, column: 26, scope: !2)
!42 = !DILocation(line: 5, column: 33, scope: !2)
!43 = !DILocalVariable(name: "Var2", scope: !2, file: !3, line: 6, type: !9)
!44 = !DILocation(line: 6, column: 23, scope: !2)
!45 = !DILocation(line: 6, column: 30, scope: !2)
!46 = !DILocalVariable(name: "Var3", scope: !2, file: !3, line: 7, type: !19)
!47 = !DILocation(line: 7, column: 25, scope: !2)
!48 = !DILocation(line: 7, column: 32, scope: !2)
!49 = !DILocation(line: 9, column: 34, scope: !2)
!50 = !DILocation(line: 9, column: 32, scope: !2)
!51 = !DILocation(line: 10, column: 36, scope: !2)
!52 = !DILocation(line: 10, column: 34, scope: !2)
!53 = !DILocation(line: 11, column: 33, scope: !2)
!54 = !DILocation(line: 11, column: 31, scope: !2)
!55 = !DILocation(line: 12, column: 35, scope: !2)
!56 = !DILocation(line: 12, column: 33, scope: !2)
!57 = !DILocalVariable(name: "Var8", scope: !2, file: !3, line: 14, type: !6)
!58 = !DILocation(line: 14, column: 23, scope: !2)
!59 = !DILocalVariable(name: "Var9", scope: !2, file: !3, line: 15, type: !8)
!60 = !DILocation(line: 15, column: 25, scope: !2)
!61 = !DILocalVariable(name: "Var10", scope: !2, file: !3, line: 16, type: !9)
!62 = !DILocation(line: 16, column: 22, scope: !2)
!63 = !DILocalVariable(name: "Var11", scope: !2, file: !3, line: 17, type: !19)
!64 = !DILocation(line: 17, column: 24, scope: !2)
!65 = !DILocation(line: 18, column: 1, scope: !2)
