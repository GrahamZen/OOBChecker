; ModuleID = '/OOBChecker/test/test02.c'
source_filename = "/OOBChecker/test/test02.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 !dbg !7 {
entry:
  %retval = alloca i32, align 4
  %x = alloca i32, align 4
  %y = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  call void @llvm.dbg.declare(metadata i32* %x, metadata !12, metadata !DIExpression()), !dbg !13
  %call = call i32 @getchar(), !dbg !14
  store i32 %call, i32* %x, align 4, !dbg !13
  call void @llvm.dbg.declare(metadata i32* %y, metadata !15, metadata !DIExpression()), !dbg !16
  %0 = load i32, i32* %x, align 4, !dbg !17
  %div = sdiv i32 5, %0, !dbg !18
  store i32 %div, i32* %y, align 4, !dbg !16
  ret i32 0, !dbg !19
}

; Function Attrs: nounwind readnone speculatable
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

declare dso_local i32 @getchar() #2

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone speculatable }
attributes #2 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!3, !4, !5}
!llvm.ident = !{!6}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 8.0.1- (branches/release_80)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, nameTableKind: None)
!1 = !DIFile(filename: "/OOBChecker/test/test02.c", directory: "/OOBChecker")
!2 = !{}
!3 = !{i32 2, !"Dwarf Version", i32 4}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = !{i32 1, !"wchar_size", i32 4}
!6 = !{!"clang version 8.0.1- (branches/release_80)"}
!7 = distinct !DISubprogram(name: "main", scope: !8, file: !8, line: 3, type: !9, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!8 = !DIFile(filename: "test/test02.c", directory: "/OOBChecker")
!9 = !DISubroutineType(types: !10)
!10 = !{!11}
!11 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!12 = !DILocalVariable(name: "x", scope: !7, file: !8, line: 4, type: !11)
!13 = !DILocation(line: 4, column: 7, scope: !7)
!14 = !DILocation(line: 4, column: 11, scope: !7)
!15 = !DILocalVariable(name: "y", scope: !7, file: !8, line: 5, type: !11)
!16 = !DILocation(line: 5, column: 7, scope: !7)
!17 = !DILocation(line: 5, column: 15, scope: !7)
!18 = !DILocation(line: 5, column: 13, scope: !7)
!19 = !DILocation(line: 6, column: 3, scope: !7)
