; ModuleID = 'gc.cpp'
source_filename = "gc.cpp"
target datalayout = "e-m:x-p:32:32-i64:64-f80:32-n8:16:32-a:0:32-S32"
target triple = "i686-pc-windows-msvc19.15.26732"

%struct.GcObj = type { %struct.GcObj*, %struct.GcObj*, i8, i8* }

$"\01??_C@_0M@EKOMOFDN@list?5start?6?$AA@" = comdat any

$"\01??_C@_07FNDMELDM@obj?$DN?$CFd?6?$AA@" = comdat any

$"\01??_C@_0BB@BOEFFFLO@is_reachable?$DN?$CFd?6?$AA@" = comdat any

$"\01??_C@_0N@KCHENNKK@typename?$DN?$CFs?6?$AA@" = comdat any

$"\01??_C@_0L@PAMKGGHN@list?5end?6?6?$AA@" = comdat any

@"\01?gc_obj_list@@3PAUGcObj@@A" = global %struct.GcObj* null, align 4
@"\01?gc_root_list@@3PAUGcObj@@A" = global %struct.GcObj* null, align 4
@"\01??_C@_0M@EKOMOFDN@list?5start?6?$AA@" = linkonce_odr unnamed_addr constant [12 x i8] c"list start\0A\00", comdat, align 1
@"\01??_C@_07FNDMELDM@obj?$DN?$CFd?6?$AA@" = linkonce_odr unnamed_addr constant [8 x i8] c"obj=%d\0A\00", comdat, align 1
@"\01??_C@_0BB@BOEFFFLO@is_reachable?$DN?$CFd?6?$AA@" = linkonce_odr unnamed_addr constant [17 x i8] c"is_reachable=%d\0A\00", comdat, align 1
@"\01??_C@_0N@KCHENNKK@typename?$DN?$CFs?6?$AA@" = linkonce_odr unnamed_addr constant [13 x i8] c"typename=%s\0A\00", comdat, align 1
@"\01??_C@_0L@PAMKGGHN@list?5end?6?6?$AA@" = linkonce_odr unnamed_addr constant [11 x i8] c"list end\0A\0A\00", comdat, align 1

; Function Attrs: noinline optnone
define void @"\01?PrintList@@YAX_N@Z"(i1 zeroext) #0 {
  %2 = alloca i8, align 1
  %3 = alloca %struct.GcObj*, align 4
  %4 = zext i1 %0 to i8
  store i8 %4, i8* %2, align 1
  %5 = load i8, i8* %2, align 1
  %6 = trunc i8 %5 to i1
  br i1 %6, label %7, label %9

; <label>:7:                                      ; preds = %1
  %8 = load %struct.GcObj*, %struct.GcObj** @"\01?gc_root_list@@3PAUGcObj@@A", align 4
  store %struct.GcObj* %8, %struct.GcObj** %3, align 4
  br label %11

; <label>:9:                                      ; preds = %1
  %10 = load %struct.GcObj*, %struct.GcObj** @"\01?gc_obj_list@@3PAUGcObj@@A", align 4
  store %struct.GcObj* %10, %struct.GcObj** %3, align 4
  br label %11

; <label>:11:                                     ; preds = %9, %7
  %12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @"\01??_C@_0M@EKOMOFDN@list?5start?6?$AA@", i32 0, i32 0))
  br label %13

; <label>:13:                                     ; preds = %40, %11
  %14 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %15 = icmp ne %struct.GcObj* %14, null
  br i1 %15, label %16, label %41

; <label>:16:                                     ; preds = %13
  %17 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %18 = ptrtoint %struct.GcObj* %17 to i32
  %19 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @"\01??_C@_07FNDMELDM@obj?$DN?$CFd?6?$AA@", i32 0, i32 0), i32 %18)
  %20 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %21 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %20, i32 0, i32 2
  %22 = load i8, i8* %21, align 4
  %23 = trunc i8 %22 to i1
  %24 = zext i1 %23 to i32
  %25 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @"\01??_C@_0BB@BOEFFFLO@is_reachable?$DN?$CFd?6?$AA@", i32 0, i32 0), i32 %24)
  %26 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %27 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %26, i32 0, i32 3
  %28 = load i8*, i8** %27, align 4
  %29 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @"\01??_C@_0N@KCHENNKK@typename?$DN?$CFs?6?$AA@", i32 0, i32 0), i8* %28)
  %30 = load i8, i8* %2, align 1
  %31 = trunc i8 %30 to i1
  br i1 %31, label %32, label %36

; <label>:32:                                     ; preds = %16
  %33 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %34 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %33, i32 0, i32 1
  %35 = load %struct.GcObj*, %struct.GcObj** %34, align 4
  store %struct.GcObj* %35, %struct.GcObj** %3, align 4
  br label %40

; <label>:36:                                     ; preds = %16
  %37 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %38 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %37, i32 0, i32 0
  %39 = load %struct.GcObj*, %struct.GcObj** %38, align 4
  store %struct.GcObj* %39, %struct.GcObj** %3, align 4
  br label %40

; <label>:40:                                     ; preds = %36, %32
  br label %13

; <label>:41:                                     ; preds = %13
  %42 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @"\01??_C@_0L@PAMKGGHN@list?5end?6?6?$AA@", i32 0, i32 0))
  ret void
}

declare i32 @printf(i8*, ...) #1

; Function Attrs: noinline optnone
define i8* @gc_malloc(i32) #0 {
  %2 = alloca i32, align 4
  %3 = alloca %struct.GcObj*, align 4
  store i32 %0, i32* %2, align 4
  call void @"\01?PrintList@@YAX_N@Z"(i1 zeroext false)
  %4 = load i32, i32* %2, align 4
  %5 = call i8* @malloc(i32 %4)
  %6 = bitcast i8* %5 to %struct.GcObj*
  store %struct.GcObj* %6, %struct.GcObj** %3, align 4
  %7 = load %struct.GcObj*, %struct.GcObj** @"\01?gc_obj_list@@3PAUGcObj@@A", align 4
  %8 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %9 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %8, i32 0, i32 0
  store %struct.GcObj* %7, %struct.GcObj** %9, align 4
  %10 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %11 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %10, i32 0, i32 1
  store %struct.GcObj* null, %struct.GcObj** %11, align 4
  %12 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %13 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %12, i32 0, i32 2
  store i8 0, i8* %13, align 4
  %14 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  store %struct.GcObj* %14, %struct.GcObj** @"\01?gc_obj_list@@3PAUGcObj@@A", align 4
  %15 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %16 = bitcast %struct.GcObj* %15 to i8*
  ret i8* %16
}

declare i8* @malloc(i32) #1

; Function Attrs: noinline optnone
define void @gc_add_root(%struct.GcObj*) #0 {
  %2 = alloca %struct.GcObj*, align 4
  store %struct.GcObj* %0, %struct.GcObj** %2, align 4
  call void @"\01?PrintList@@YAX_N@Z"(i1 zeroext true)
  %3 = load %struct.GcObj*, %struct.GcObj** @"\01?gc_root_list@@3PAUGcObj@@A", align 4
  %4 = load %struct.GcObj*, %struct.GcObj** %2, align 4
  %5 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %4, i32 0, i32 1
  store %struct.GcObj* %3, %struct.GcObj** %5, align 4
  %6 = load %struct.GcObj*, %struct.GcObj** %2, align 4
  store %struct.GcObj* %6, %struct.GcObj** @"\01?gc_root_list@@3PAUGcObj@@A", align 4
  ret void
}

attributes #0 = { noinline optnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"NumRegisterParameters", i32 0}
!1 = !{i32 1, !"wchar_size", i32 2}
!2 = !{!"clang version 6.0.0 (tags/RELEASE_600/final)"}
