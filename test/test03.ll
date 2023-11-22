; ModuleID = '/OOBChecker/test/test03.c'
source_filename = "/OOBChecker/test/test03.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 !dbg !7 {
entry:
  %retval = alloca i32, align 4
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  %d = alloca i32, align 4
  %e = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  call void @llvm.dbg.declare(metadata i32* %a, metadata !12, metadata !DIExpression()), !dbg !13
  store i32 0, i32* %a, align 4, !dbg !13
  call void @llvm.dbg.declare(metadata i32* %b, metadata !14, metadata !DIExpression()), !dbg !15
  store i32 0, i32* %b, align 4, !dbg !15
  call void @llvm.dbg.declare(metadata i32* %c, metadata !16, metadata !DIExpression()), !dbg !17
  %0 = load i32, i32* %a, align 4, !dbg !18
  %1 = load i32, i32* %b, align 4, !dbg !19
  %cmp = icmp eq i32 %0, %1, !dbg !20
  %conv = zext i1 %cmp to i32, !dbg !20
  store i32 %conv, i32* %c, align 4, !dbg !17
  call void @llvm.dbg.declare(metadata i32* %d, metadata !21, metadata !DIExpression()), !dbg !22
  %2 = load i32, i32* %b, align 4, !dbg !23
  %3 = load i32, i32* %c, align 4, !dbg !24
  %div = sdiv i32 %2, %3, !dbg !25
  store i32 %div, i32* %d, align 4, !dbg !22
  call void @llvm.dbg.declare(metadata i32* %e, metadata !26, metadata !DIExpression()), !dbg !27
  %4 = load i32, i32* %d, align 4, !dbg !28
  %5 = load i32, i32* %d, align 4, !dbg !29
  %div1 = sdiv i32 %4, %5, !dbg !30
  store i32 %div1, i32* %e, align 4, !dbg !27
  ret i32 0, !dbg !31
}

; Function Attrs: nounwind readnone speculatable
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone speculatable }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!3, !4, !5}
!llvm.ident = !{!6}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 8.0.1- (branches/release_80)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, nameTableKind: None)
!1 = !DIFile(filename: "/OOBChecker/test/test03.c", directory: "/OOBChecker")
!2 = !{}
!3 = !{i32 2, !"Dwarf Version", i32 4}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = !{i32 1, !"wchar_size", i32 4}
!6 = !{!"clang version 8.0.1- (branches/release_80)"}
!7 = distinct !DISubprogram(name: "main", scope: !8, file: !8, line: 1, type: !9, scopeLine: 1, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!8 = !DIFile(filename: "test/test03.c", directory: "/OOBChecker")
!9 = !DISubroutineType(types: !10)
!10 = !{!11}
!11 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!12 = !DILocalVariable(name: "a", scope: !7, file: !8, line: 2, type: !11)
!13 = !DILocation(line: 2, column: 7, scope: !7)
!14 = !DILocalVariable(name: "b", scope: !7, file: !8, line: 3, type: !11)
!15 = !DILocation(line: 3, column: 7, scope: !7)
!16 = !DILocalVariable(name: "c", scope: !7, file: !8, line: 4, type: !11)
!17 = !DILocation(line: 4, column: 7, scope: !7)
!18 = !DILocation(line: 4, column: 11, scope: !7)
!19 = !DILocation(line: 4, column: 16, scope: !7)
!20 = !DILocation(line: 4, column: 13, scope: !7)
!21 = !DILocalVariable(name: "d", scope: !7, file: !8, line: 5, type: !11)
!22 = !DILocation(line: 5, column: 7, scope: !7)
!23 = !DILocation(line: 5, column: 11, scope: !7)
!24 = !DILocation(line: 5, column: 15, scope: !7)
!25 = !DILocation(line: 5, column: 13, scope: !7)
!26 = !DILocalVariable(name: "e", scope: !7, file: !8, line: 6, type: !11)
!27 = !DILocation(line: 6, column: 7, scope: !7)
!28 = !DILocation(line: 6, column: 11, scope: !7)
!29 = !DILocation(line: 6, column: 15, scope: !7)
!30 = !DILocation(line: 6, column: 13, scope: !7)
!31 = !DILocation(line: 7, column: 3, scope: !7)
