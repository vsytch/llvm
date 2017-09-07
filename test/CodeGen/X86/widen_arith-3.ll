; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc < %s -mtriple=i686-unknown-unknown -mattr=+sse4.2 -post-RA-scheduler=true | FileCheck %s

; Widen a v3i16 to v8i16 to do a vector add

@.str = internal constant [4 x i8] c"%d \00"
@.str1 = internal constant [2 x i8] c"\0A\00"

define void @update(<3 x i16>* %dst, <3 x i16>* %src, i32 %n) nounwind {
; CHECK-LABEL: update:
; CHECK:       # BB#0: # %entry
; CHECK-NEXT:    pushl %ebp
; CHECK-NEXT:    movl %esp, %ebp
; CHECK-NEXT:    andl $-8, %esp
; CHECK-NEXT:    subl $40, %esp
; CHECK-NEXT:    movl {{\.LCPI.*}}, %eax
; CHECK-NEXT:    movdqa {{.*#+}} xmm1 = [0,1,4,5,8,9,12,13,8,9,12,13,12,13,14,15]
; CHECK-NEXT:    pcmpeqd %xmm0, %xmm0
<<<<<<< HEAD
=======
; CHECK-NEXT:    movw $1, {{[0-9]+}}(%esp)
>>>>>>> 088a118f83a6aef379d0de80ceb9aa764854b9d0
; CHECK-NEXT:    movl $0, {{[0-9]+}}(%esp)
; CHECK-NEXT:    movl %eax, {{[0-9]+}}(%esp)
; CHECK-NEXT:    jmp .LBB0_1
; CHECK-NEXT:    .p2align 4, 0x90
; CHECK-NEXT:  .LBB0_2: # %forbody
; CHECK-NEXT:    # in Loop: Header=BB0_1 Depth=1
; CHECK-NEXT:    movl {{[0-9]+}}(%esp), %eax
; CHECK-NEXT:    movl 12(%ebp), %edx
; CHECK-NEXT:    movl 8(%ebp), %ecx
; CHECK-NEXT:    movd {{.*#+}} xmm2 = mem[0],zero,zero,zero
; CHECK-NEXT:    pmovzxwd {{.*#+}} xmm2 = xmm2[0],zero,xmm2[1],zero,xmm2[2],zero,xmm2[3],zero
; CHECK-NEXT:    pinsrd $2, 4(%edx,%eax,8), %xmm2
; CHECK-NEXT:    psubd %xmm0, %xmm2
; CHECK-NEXT:    pextrw $4, %xmm2, 4(%ecx,%eax,8)
; CHECK-NEXT:    pshufb %xmm1, %xmm2
; CHECK-NEXT:    movd %xmm2, (%ecx,%eax,8)
; CHECK-NEXT:    incl {{[0-9]+}}(%esp)
; CHECK-NEXT:  .LBB0_1: # %forcond
; CHECK-NEXT:    # =>This Inner Loop Header: Depth=1
; CHECK-NEXT:    movl {{[0-9]+}}(%esp), %eax
; CHECK-NEXT:    cmpl 16(%ebp), %eax
; CHECK-NEXT:    jl .LBB0_2
; CHECK-NEXT:  # BB#3: # %afterfor
; CHECK-NEXT:    movl %ebp, %esp
; CHECK-NEXT:    popl %ebp
; CHECK-NEXT:    retl
entry:
	%dst.addr = alloca <3 x i16>*
	%src.addr = alloca <3 x i16>*
	%n.addr = alloca i32
	%v = alloca <3 x i16>, align 8
	%i = alloca i32, align 4
	store <3 x i16>* %dst, <3 x i16>** %dst.addr
	store <3 x i16>* %src, <3 x i16>** %src.addr
	store i32 %n, i32* %n.addr
	store <3 x i16> < i16 1, i16 1, i16 1 >, <3 x i16>* %v
	store i32 0, i32* %i
	br label %forcond

forcond:
	%tmp = load i32, i32* %i
	%tmp1 = load i32, i32* %n.addr
	%cmp = icmp slt i32 %tmp, %tmp1
	br i1 %cmp, label %forbody, label %afterfor

forbody:
	%tmp2 = load i32, i32* %i
	%tmp3 = load <3 x i16>*, <3 x i16>** %dst.addr
	%arrayidx = getelementptr <3 x i16>, <3 x i16>* %tmp3, i32 %tmp2
	%tmp4 = load i32, i32* %i
	%tmp5 = load <3 x i16>*, <3 x i16>** %src.addr
	%arrayidx6 = getelementptr <3 x i16>, <3 x i16>* %tmp5, i32 %tmp4
	%tmp7 = load <3 x i16>, <3 x i16>* %arrayidx6
	%add = add <3 x i16> %tmp7, < i16 1, i16 1, i16 1 >
	store <3 x i16> %add, <3 x i16>* %arrayidx
	br label %forinc

forinc:
	%tmp8 = load i32, i32* %i
	%inc = add i32 %tmp8, 1
	store i32 %inc, i32* %i
	br label %forcond

afterfor:
	ret void
}

