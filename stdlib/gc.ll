; ModuleID = 'gc.cpp'
source_filename = "gc.cpp"
target datalayout = "e-m:x-p:32:32-i64:64-f80:32-n8:16:32-a:0:32-S32"
target triple = "i686-pc-windows-msvc19.15.26732"

%struct.GcObj = type { i8*, i8, %struct.GcObj* }

$"\01??0GcObj@@QAE@XZ" = comdat any

$"\01??_C@_0M@EKOMOFDN@list?5start?6?$AA@" = comdat any

$"\01??_C@_07FNDMELDM@obj?$DN?$CFd?6?$AA@" = comdat any

$"\01??_C@_0BB@BOEFFFLO@is_reachable?$DN?$CFd?6?$AA@" = comdat any

$"\01??_C@_09CFCANAIO@list?5end?6?$AA@" = comdat any

@"\01?gc_obj_list@@3PAUGcObj@@A" = global %struct.GcObj* null, align 4
@"\01??_C@_0M@EKOMOFDN@list?5start?6?$AA@" = linkonce_odr unnamed_addr constant [12 x i8] c"list start\0A\00", comdat, align 1
@"\01??_C@_07FNDMELDM@obj?$DN?$CFd?6?$AA@" = linkonce_odr unnamed_addr constant [8 x i8] c"obj=%d\0A\00", comdat, align 1
@"\01??_C@_0BB@BOEFFFLO@is_reachable?$DN?$CFd?6?$AA@" = linkonce_odr unnamed_addr constant [17 x i8] c"is_reachable=%d\0A\00", comdat, align 1
@"\01??_C@_09CFCANAIO@list?5end?6?$AA@" = linkonce_odr unnamed_addr constant [10 x i8] c"list end\0A\00", comdat, align 1

; Function Attrs: noinline optnone
define void @"\01?PrintList@@YAXPAUGcObj@@@Z"(%struct.GcObj*) #0 {
  %2 = alloca %struct.GcObj*, align 4
  store %struct.GcObj* %0, %struct.GcObj** %2, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @"\01??_C@_0M@EKOMOFDN@list?5start?6?$AA@", i32 0, i32 0))
  br label %4

; <label>:4:                                      ; preds = %7, %1
  %5 = load %struct.GcObj*, %struct.GcObj** %2, align 4
  %6 = icmp ne %struct.GcObj* %5, null
  br i1 %6, label %7, label %22

; <label>:7:                                      ; preds = %4
  %8 = load %struct.GcObj*, %struct.GcObj** %2, align 4
  %9 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %8, i32 0, i32 0
  %10 = load i8*, i8** %9, align 4
  %11 = ptrtoint i8* %10 to i32
  %12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @"\01??_C@_07FNDMELDM@obj?$DN?$CFd?6?$AA@", i32 0, i32 0), i32 %11)
  %13 = load %struct.GcObj*, %struct.GcObj** %2, align 4
  %14 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %13, i32 0, i32 1
  %15 = load i8, i8* %14, align 4
  %16 = trunc i8 %15 to i1
  %17 = zext i1 %16 to i32
  %18 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @"\01??_C@_0BB@BOEFFFLO@is_reachable?$DN?$CFd?6?$AA@", i32 0, i32 0), i32 %17)
  %19 = load %struct.GcObj*, %struct.GcObj** %2, align 4
  %20 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %19, i32 0, i32 2
  %21 = load %struct.GcObj*, %struct.GcObj** %20, align 4
  store %struct.GcObj* %21, %struct.GcObj** %2, align 4
  br label %4

; <label>:22:                                     ; preds = %4
  %23 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @"\01??_C@_09CFCANAIO@list?5end?6?$AA@", i32 0, i32 0))
  ret void
}

declare i32 @printf(i8*, ...) #1

; Function Attrs: noinline optnone
define i8* @gc_malloc(i32) #0 {
  %2 = alloca i32, align 4
  %3 = alloca i8*, align 4
  %4 = alloca %struct.GcObj*, align 4
  %5 = alloca %struct.GcObj*, align 4
  store i32 %0, i32* %2, align 4
  %6 = load i32, i32* %2, align 4
  %7 = call i8* @malloc(i32 %6)
  store i8* %7, i8** %3, align 4
  %8 = load %struct.GcObj*, %struct.GcObj** @"\01?gc_obj_list@@3PAUGcObj@@A", align 4
  store %struct.GcObj* %8, %struct.GcObj** %4, align 4
  %9 = call i8* @"\01??2@YAPAXI@Z"(i32 12) #4
  %10 = bitcast i8* %9 to %struct.GcObj*
  %11 = call x86_thiscallcc %struct.GcObj* @"\01??0GcObj@@QAE@XZ"(%struct.GcObj* %10) #5
  store %struct.GcObj* %10, %struct.GcObj** %5, align 4
  %12 = load i8*, i8** %3, align 4
  %13 = load %struct.GcObj*, %struct.GcObj** %5, align 4
  %14 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %13, i32 0, i32 0
  store i8* %12, i8** %14, align 4
  %15 = load %struct.GcObj*, %struct.GcObj** %4, align 4
  %16 = load %struct.GcObj*, %struct.GcObj** %5, align 4
  %17 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %16, i32 0, i32 2
  store %struct.GcObj* %15, %struct.GcObj** %17, align 4
  %18 = load %struct.GcObj*, %struct.GcObj** %5, align 4
  store %struct.GcObj* %18, %struct.GcObj** @"\01?gc_obj_list@@3PAUGcObj@@A", align 4
  %19 = load %struct.GcObj*, %struct.GcObj** @"\01?gc_obj_list@@3PAUGcObj@@A", align 4
  call void @"\01?PrintList@@YAXPAUGcObj@@@Z"(%struct.GcObj* %19)
  %20 = load i8*, i8** %3, align 4
  ret i8* %20
}

declare i8* @malloc(i32) #1

; Function Attrs: nobuiltin
declare noalias i8* @"\01??2@YAPAXI@Z"(i32) #2

; Function Attrs: noinline nounwind optnone
define linkonce_odr x86_thiscallcc %struct.GcObj* @"\01??0GcObj@@QAE@XZ"(%struct.GcObj* returned) unnamed_addr #3 comdat align 2 {
  %2 = alloca %struct.GcObj*, align 4
  store %struct.GcObj* %0, %struct.GcObj** %2, align 4
  %3 = load %struct.GcObj*, %struct.GcObj** %2, align 4
  %4 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %3, i32 0, i32 1
  store i8 0, i8* %4, align 4
  %5 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %3, i32 0, i32 2
  store %struct.GcObj* null, %struct.GcObj** %5, align 4
  ret %struct.GcObj* %3
}

attributes #0 = { noinline optnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nobuiltin "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { noinline nounwind optnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { builtin }
attributes #5 = { nounwind }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"NumRegisterParameters", i32 0}
!1 = !{i32 1, !"wchar_size", i32 2}
!2 = !{!"clang version 6.0.0 (tags/RELEASE_600/final)"}
