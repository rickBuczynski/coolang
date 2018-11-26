; ModuleID = 'gc.cpp'
source_filename = "gc.cpp"
target datalayout = "e-m:x-p:32:32-i64:64-f80:32-n8:16:32-a:0:32-S32"
target triple = "i686-pc-windows-msvc19.15.26732"

$"\01??_C@_09MPAGEDNP@gc_malloc?$AA@" = comdat any

$"\01??_C@_09DEHHIH@hello?5?$CFs?6?$AA@" = comdat any

@"\01??_C@_09MPAGEDNP@gc_malloc?$AA@" = linkonce_odr unnamed_addr constant [10 x i8] c"gc_malloc\00", comdat, align 1
@"\01??_C@_09DEHHIH@hello?5?$CFs?6?$AA@" = linkonce_odr unnamed_addr constant [10 x i8] c"hello %s\0A\00", comdat, align 1

; Function Attrs: noinline optnone
define i8* @gc_malloc(i32) #0 {
  %2 = alloca i32, align 4
  store i32 %0, i32* %2, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @"\01??_C@_09DEHHIH@hello?5?$CFs?6?$AA@", i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @"\01??_C@_09MPAGEDNP@gc_malloc?$AA@", i32 0, i32 0))
  %4 = load i32, i32* %2, align 4
  %5 = call i8* @malloc(i32 %4)
  ret i8* %5
}

declare i32 @printf(i8*, ...) #1

declare i8* @malloc(i32) #1

attributes #0 = { noinline optnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"NumRegisterParameters", i32 0}
!1 = !{i32 1, !"wchar_size", i32 2}
!2 = !{!"clang version 6.0.0 (tags/RELEASE_600/final)"}
