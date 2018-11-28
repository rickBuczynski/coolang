; ModuleID = 'gc.cpp'
source_filename = "gc.cpp"
target datalayout = "e-m:x-p:32:32-i64:64-f80:32-n8:16:32-a:0:32-S32"
target triple = "i686-pc-windows-msvc19.15.26732"

%struct.GcObj = type { %struct.GcObj*, %struct.GcObj*, i8, i8* }

$"\01??_C@_08NJAKNIEC@gc?5roots?$AA@" = comdat any

$"\01??_C@_07DGBOGBKN@gc?5objs?$AA@" = comdat any

$"\01??_C@_09EBJFJNG@?$CFs?5start?6?$AA@" = comdat any

$"\01??_C@_07FNDMELDM@obj?$DN?$CFd?6?$AA@" = comdat any

$"\01??_C@_0BD@HGGAKDEI@?5?5is_reachable?$DN?$CFd?6?$AA@" = comdat any

$"\01??_C@_0P@DPHCNOIJ@?5?5typename?$DN?$CFs?6?$AA@" = comdat any

$"\01??_C@_08BPFNJJP@?$CFs?5end?6?6?$AA@" = comdat any

@"\01?gc_obj_list@@3PAUGcObj@@A" = global %struct.GcObj* null, align 4
@"\01?gc_root_list@@3PAUGcObj@@A" = global %struct.GcObj* null, align 4
@"\01??_C@_08NJAKNIEC@gc?5roots?$AA@" = linkonce_odr unnamed_addr constant [9 x i8] c"gc roots\00", comdat, align 1
@"\01??_C@_07DGBOGBKN@gc?5objs?$AA@" = linkonce_odr unnamed_addr constant [8 x i8] c"gc objs\00", comdat, align 1
@"\01??_C@_09EBJFJNG@?$CFs?5start?6?$AA@" = linkonce_odr unnamed_addr constant [10 x i8] c"%s start\0A\00", comdat, align 1
@"\01??_C@_07FNDMELDM@obj?$DN?$CFd?6?$AA@" = linkonce_odr unnamed_addr constant [8 x i8] c"obj=%d\0A\00", comdat, align 1
@"\01??_C@_0BD@HGGAKDEI@?5?5is_reachable?$DN?$CFd?6?$AA@" = linkonce_odr unnamed_addr constant [19 x i8] c"  is_reachable=%d\0A\00", comdat, align 1
@"\01??_C@_0P@DPHCNOIJ@?5?5typename?$DN?$CFs?6?$AA@" = linkonce_odr unnamed_addr constant [15 x i8] c"  typename=%s\0A\00", comdat, align 1
@"\01??_C@_08BPFNJJP@?$CFs?5end?6?6?$AA@" = linkonce_odr unnamed_addr constant [9 x i8] c"%s end\0A\0A\00", comdat, align 1

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
  %12 = load i8, i8* %2, align 1
  %13 = trunc i8 %12 to i1
  %14 = zext i1 %13 to i64
  %15 = select i1 %13, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @"\01??_C@_08NJAKNIEC@gc?5roots?$AA@", i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @"\01??_C@_07DGBOGBKN@gc?5objs?$AA@", i32 0, i32 0)
  %16 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @"\01??_C@_09EBJFJNG@?$CFs?5start?6?$AA@", i32 0, i32 0), i8* %15)
  br label %17

; <label>:17:                                     ; preds = %44, %11
  %18 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %19 = icmp ne %struct.GcObj* %18, null
  br i1 %19, label %20, label %45

; <label>:20:                                     ; preds = %17
  %21 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %22 = ptrtoint %struct.GcObj* %21 to i32
  %23 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @"\01??_C@_07FNDMELDM@obj?$DN?$CFd?6?$AA@", i32 0, i32 0), i32 %22)
  %24 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %25 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %24, i32 0, i32 2
  %26 = load i8, i8* %25, align 4
  %27 = trunc i8 %26 to i1
  %28 = zext i1 %27 to i32
  %29 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @"\01??_C@_0BD@HGGAKDEI@?5?5is_reachable?$DN?$CFd?6?$AA@", i32 0, i32 0), i32 %28)
  %30 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %31 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %30, i32 0, i32 3
  %32 = load i8*, i8** %31, align 4
  %33 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @"\01??_C@_0P@DPHCNOIJ@?5?5typename?$DN?$CFs?6?$AA@", i32 0, i32 0), i8* %32)
  %34 = load i8, i8* %2, align 1
  %35 = trunc i8 %34 to i1
  br i1 %35, label %36, label %40

; <label>:36:                                     ; preds = %20
  %37 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %38 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %37, i32 0, i32 1
  %39 = load %struct.GcObj*, %struct.GcObj** %38, align 4
  store %struct.GcObj* %39, %struct.GcObj** %3, align 4
  br label %44

; <label>:40:                                     ; preds = %20
  %41 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %42 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %41, i32 0, i32 0
  %43 = load %struct.GcObj*, %struct.GcObj** %42, align 4
  store %struct.GcObj* %43, %struct.GcObj** %3, align 4
  br label %44

; <label>:44:                                     ; preds = %40, %36
  br label %17

; <label>:45:                                     ; preds = %17
  %46 = load i8, i8* %2, align 1
  %47 = trunc i8 %46 to i1
  %48 = zext i1 %47 to i64
  %49 = select i1 %47, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @"\01??_C@_08NJAKNIEC@gc?5roots?$AA@", i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @"\01??_C@_07DGBOGBKN@gc?5objs?$AA@", i32 0, i32 0)
  %50 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @"\01??_C@_08BPFNJJP@?$CFs?5end?6?6?$AA@", i32 0, i32 0), i8* %49)
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
