// RUN: not llvm-mc -filetype=obj -triple i386-pc-win32 %s 2>&1 | FileCheck %s

<<<<<<< HEAD
// CHECK: symbol '__ImageBase' can not be undefined in a subtraction expression

=======
>>>>>>> 088a118f83a6aef379d0de80ceb9aa764854b9d0
        .data
_x:
// CHECK: [[@LINE+1]]:{{[0-9]+}}: error: symbol '__ImageBase' can not be undefined in a subtraction expression
        .long   _x-__ImageBase
