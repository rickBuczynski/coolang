; ModuleID = '.\gc.cpp'
source_filename = ".\5Cgc.cpp"
target datalayout = "e-m:x-p:32:32-i64:64-f80:32-n8:16:32-a:0:32-S32"
target triple = "i386-pc-windows-msvc19.16.27024"

%class.GcList = type { %struct.GcObj* }
%struct.GcObj = type { %struct.GcObj*, %struct.GcObj*, %struct.GcObj*, %struct.GcObj*, i8, i8* }
%class.GcList.0 = type { %struct.GcObj* }

$"?PrintList@?$GcList@VGcObjList@@@@QAEXXZ" = comdat any

$"?PushFront@?$GcList@VGcObjList@@@@QAEXPAUGcObj@@@Z" = comdat any

$"?PushFront@?$GcList@VGcRootList@@@@QAEXPAUGcObj@@@Z" = comdat any

$"?PrintList@?$GcList@VGcRootList@@@@QAEXXZ" = comdat any

$"?Remove@?$GcList@VGcRootList@@@@QAEXPAUGcObj@@@Z" = comdat any

$"?ListName@GcObjList@@SAPBDXZ" = comdat any

$"?GetPrev@GcObjList@@SAPAUGcObj@@PAU2@@Z" = comdat any

$"?GetNext@GcObjList@@SAPAUGcObj@@PAU2@@Z" = comdat any

$"?SetNext@GcObjList@@SAXPAUGcObj@@0@Z" = comdat any

$"?SetPrev@GcObjList@@SAXPAUGcObj@@0@Z" = comdat any

$"?SetNext@GcRootList@@SAXPAUGcObj@@0@Z" = comdat any

$"?SetPrev@GcRootList@@SAXPAUGcObj@@0@Z" = comdat any

$"?ListName@GcRootList@@SAPBDXZ" = comdat any

$"?GetPrev@GcRootList@@SAPAUGcObj@@PAU2@@Z" = comdat any

$"?GetNext@GcRootList@@SAPAUGcObj@@PAU2@@Z" = comdat any

$"??_C@_09EBJFJNG@?$CFs?5start?6?$AA@" = comdat any

$"??_C@_07FNDMELDM@obj?$DN?$CFd?6?$AA@" = comdat any

$"??_C@_0BD@HGGAKDEI@?5?5is_reachable?$DN?$CFd?6?$AA@" = comdat any

$"??_C@_0P@DPHCNOIJ@?5?5typename?$DN?$CFs?6?$AA@" = comdat any

$"??_C@_0CO@HGIHIH@?5?5BADBADBADBADBADBADBADBADBADBAD@" = comdat any

$"??_C@_08BPFNJJP@?$CFs?5end?6?6?$AA@" = comdat any

$"??_C@_07DGBOGBKN@gc?5objs?$AA@" = comdat any

$"??_C@_08NJAKNIEC@gc?5roots?$AA@" = comdat any

$"??_C@_0CP@HIJCJBBI@Tried?5to?5remove?5an?5obj?5that?8s?5no@" = comdat any

@"?gc_obj_list@@3V?$GcList@VGcObjList@@@@A" = dso_local global %class.GcList zeroinitializer, align 4
@"?gc_root_list@@3V?$GcList@VGcRootList@@@@A" = dso_local global %class.GcList.0 zeroinitializer, align 4
@"??_C@_09EBJFJNG@?$CFs?5start?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [10 x i8] c"%s start\0A\00", comdat, align 1
@"??_C@_07FNDMELDM@obj?$DN?$CFd?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [8 x i8] c"obj=%d\0A\00", comdat, align 1
@"??_C@_0BD@HGGAKDEI@?5?5is_reachable?$DN?$CFd?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [19 x i8] c"  is_reachable=%d\0A\00", comdat, align 1
@"??_C@_0P@DPHCNOIJ@?5?5typename?$DN?$CFs?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [15 x i8] c"  typename=%s\0A\00", comdat, align 1
@"??_C@_0CO@HGIHIH@?5?5BADBADBADBADBADBADBADBADBADBAD@" = linkonce_odr dso_local unnamed_addr constant [46 x i8] c"  BADBADBADBADBADBADBADBADBADBADBADBADBADBAD\0A\00", comdat, align 1
@"??_C@_08BPFNJJP@?$CFs?5end?6?6?$AA@" = linkonce_odr dso_local unnamed_addr constant [9 x i8] c"%s end\0A\0A\00", comdat, align 1
@"??_C@_07DGBOGBKN@gc?5objs?$AA@" = linkonce_odr dso_local unnamed_addr constant [8 x i8] c"gc objs\00", comdat, align 1
@"??_C@_08NJAKNIEC@gc?5roots?$AA@" = linkonce_odr dso_local unnamed_addr constant [9 x i8] c"gc roots\00", comdat, align 1
@"??_C@_0CP@HIJCJBBI@Tried?5to?5remove?5an?5obj?5that?8s?5no@" = linkonce_odr dso_local unnamed_addr constant [47 x i8] c"Tried to remove an obj that's not in list: %s\0A\00", comdat, align 1

; Function Attrs: noinline optnone
define dso_local void @print_gc_obj_list() #0 {
  call x86_thiscallcc void @"?PrintList@?$GcList@VGcObjList@@@@QAEXXZ"(%class.GcList* @"?gc_obj_list@@3V?$GcList@VGcObjList@@@@A")
  ret void
}

; Function Attrs: noinline optnone
define linkonce_odr dso_local x86_thiscallcc void @"?PrintList@?$GcList@VGcObjList@@@@QAEXXZ"(%class.GcList*) #0 comdat align 2 {
  %2 = alloca %class.GcList*, align 4
  %3 = alloca %struct.GcObj*, align 4
  %4 = alloca %struct.GcObj*, align 4
  store %class.GcList* %0, %class.GcList** %2, align 4
  %5 = load %class.GcList*, %class.GcList** %2, align 4
  %6 = getelementptr inbounds %class.GcList, %class.GcList* %5, i32 0, i32 0
  %7 = load %struct.GcObj*, %struct.GcObj** %6, align 4
  store %struct.GcObj* %7, %struct.GcObj** %3, align 4
  store %struct.GcObj* null, %struct.GcObj** %4, align 4
  %8 = call i8* @"?ListName@GcObjList@@SAPBDXZ"()
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @"??_C@_09EBJFJNG@?$CFs?5start?6?$AA@", i32 0, i32 0), i8* %8)
  br label %10

; <label>:10:                                     ; preds = %33, %1
  %11 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %12 = icmp ne %struct.GcObj* %11, null
  br i1 %12, label %13, label %37

; <label>:13:                                     ; preds = %10
  %14 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %15 = ptrtoint %struct.GcObj* %14 to i32
  %16 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @"??_C@_07FNDMELDM@obj?$DN?$CFd?6?$AA@", i32 0, i32 0), i32 %15)
  %17 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %18 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %17, i32 0, i32 4
  %19 = load i8, i8* %18, align 4
  %20 = trunc i8 %19 to i1
  %21 = zext i1 %20 to i32
  %22 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @"??_C@_0BD@HGGAKDEI@?5?5is_reachable?$DN?$CFd?6?$AA@", i32 0, i32 0), i32 %21)
  %23 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %24 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %23, i32 0, i32 5
  %25 = load i8*, i8** %24, align 4
  %26 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @"??_C@_0P@DPHCNOIJ@?5?5typename?$DN?$CFs?6?$AA@", i32 0, i32 0), i8* %25)
  %27 = load %struct.GcObj*, %struct.GcObj** %4, align 4
  %28 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %29 = call %struct.GcObj* @"?GetPrev@GcObjList@@SAPAUGcObj@@PAU2@@Z"(%struct.GcObj* %28)
  %30 = icmp ne %struct.GcObj* %27, %29
  br i1 %30, label %31, label %33

; <label>:31:                                     ; preds = %13
  %32 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([46 x i8], [46 x i8]* @"??_C@_0CO@HGIHIH@?5?5BADBADBADBADBADBADBADBADBADBAD@", i32 0, i32 0))
  br label %33

; <label>:33:                                     ; preds = %31, %13
  %34 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  store %struct.GcObj* %34, %struct.GcObj** %4, align 4
  %35 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %36 = call %struct.GcObj* @"?GetNext@GcObjList@@SAPAUGcObj@@PAU2@@Z"(%struct.GcObj* %35)
  store %struct.GcObj* %36, %struct.GcObj** %3, align 4
  br label %10

; <label>:37:                                     ; preds = %10
  %38 = call i8* @"?ListName@GcObjList@@SAPBDXZ"()
  %39 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @"??_C@_08BPFNJJP@?$CFs?5end?6?6?$AA@", i32 0, i32 0), i8* %38)
  ret void
}

; Function Attrs: noinline optnone
define dso_local i8* @gc_malloc(i32) #0 {
  %2 = alloca i32, align 4
  %3 = alloca %struct.GcObj*, align 4
  store i32 %0, i32* %2, align 4
  %4 = load i32, i32* %2, align 4
  %5 = call i8* @malloc(i32 %4)
  %6 = bitcast i8* %5 to %struct.GcObj*
  store %struct.GcObj* %6, %struct.GcObj** %3, align 4
  %7 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %8 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %7, i32 0, i32 0
  store %struct.GcObj* null, %struct.GcObj** %8, align 4
  %9 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %10 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %9, i32 0, i32 1
  store %struct.GcObj* null, %struct.GcObj** %10, align 4
  %11 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %12 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %11, i32 0, i32 2
  store %struct.GcObj* null, %struct.GcObj** %12, align 4
  %13 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %14 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %13, i32 0, i32 3
  store %struct.GcObj* null, %struct.GcObj** %14, align 4
  %15 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %16 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %15, i32 0, i32 4
  store i8 0, i8* %16, align 4
  %17 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  call x86_thiscallcc void @"?PushFront@?$GcList@VGcObjList@@@@QAEXPAUGcObj@@@Z"(%class.GcList* @"?gc_obj_list@@3V?$GcList@VGcObjList@@@@A", %struct.GcObj* %17)
  %18 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %19 = bitcast %struct.GcObj* %18 to i8*
  ret i8* %19
}

declare dso_local i8* @malloc(i32) #1

; Function Attrs: noinline optnone
define linkonce_odr dso_local x86_thiscallcc void @"?PushFront@?$GcList@VGcObjList@@@@QAEXPAUGcObj@@@Z"(%class.GcList*, %struct.GcObj*) #0 comdat align 2 {
  %3 = alloca %struct.GcObj*, align 4
  %4 = alloca %class.GcList*, align 4
  store %struct.GcObj* %1, %struct.GcObj** %3, align 4
  store %class.GcList* %0, %class.GcList** %4, align 4
  %5 = load %class.GcList*, %class.GcList** %4, align 4
  %6 = getelementptr inbounds %class.GcList, %class.GcList* %5, i32 0, i32 0
  %7 = load %struct.GcObj*, %struct.GcObj** %6, align 4
  %8 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  call void @"?SetNext@GcObjList@@SAXPAUGcObj@@0@Z"(%struct.GcObj* %8, %struct.GcObj* %7)
  %9 = getelementptr inbounds %class.GcList, %class.GcList* %5, i32 0, i32 0
  %10 = load %struct.GcObj*, %struct.GcObj** %9, align 4
  %11 = icmp ne %struct.GcObj* %10, null
  br i1 %11, label %12, label %16

; <label>:12:                                     ; preds = %2
  %13 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %14 = getelementptr inbounds %class.GcList, %class.GcList* %5, i32 0, i32 0
  %15 = load %struct.GcObj*, %struct.GcObj** %14, align 4
  call void @"?SetPrev@GcObjList@@SAXPAUGcObj@@0@Z"(%struct.GcObj* %15, %struct.GcObj* %13)
  br label %16

; <label>:16:                                     ; preds = %12, %2
  %17 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %18 = getelementptr inbounds %class.GcList, %class.GcList* %5, i32 0, i32 0
  store %struct.GcObj* %17, %struct.GcObj** %18, align 4
  ret void
}

; Function Attrs: noinline optnone
define dso_local void @gc_add_root(%struct.GcObj*) #0 {
  %2 = alloca %struct.GcObj*, align 4
  store %struct.GcObj* %0, %struct.GcObj** %2, align 4
  %3 = load %struct.GcObj*, %struct.GcObj** %2, align 4
  call x86_thiscallcc void @"?PushFront@?$GcList@VGcRootList@@@@QAEXPAUGcObj@@@Z"(%class.GcList.0* @"?gc_root_list@@3V?$GcList@VGcRootList@@@@A", %struct.GcObj* %3)
  call x86_thiscallcc void @"?PrintList@?$GcList@VGcRootList@@@@QAEXXZ"(%class.GcList.0* @"?gc_root_list@@3V?$GcList@VGcRootList@@@@A")
  ret void
}

; Function Attrs: noinline optnone
define linkonce_odr dso_local x86_thiscallcc void @"?PushFront@?$GcList@VGcRootList@@@@QAEXPAUGcObj@@@Z"(%class.GcList.0*, %struct.GcObj*) #0 comdat align 2 {
  %3 = alloca %struct.GcObj*, align 4
  %4 = alloca %class.GcList.0*, align 4
  store %struct.GcObj* %1, %struct.GcObj** %3, align 4
  store %class.GcList.0* %0, %class.GcList.0** %4, align 4
  %5 = load %class.GcList.0*, %class.GcList.0** %4, align 4
  %6 = getelementptr inbounds %class.GcList.0, %class.GcList.0* %5, i32 0, i32 0
  %7 = load %struct.GcObj*, %struct.GcObj** %6, align 4
  %8 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  call void @"?SetNext@GcRootList@@SAXPAUGcObj@@0@Z"(%struct.GcObj* %8, %struct.GcObj* %7)
  %9 = getelementptr inbounds %class.GcList.0, %class.GcList.0* %5, i32 0, i32 0
  %10 = load %struct.GcObj*, %struct.GcObj** %9, align 4
  %11 = icmp ne %struct.GcObj* %10, null
  br i1 %11, label %12, label %16

; <label>:12:                                     ; preds = %2
  %13 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %14 = getelementptr inbounds %class.GcList.0, %class.GcList.0* %5, i32 0, i32 0
  %15 = load %struct.GcObj*, %struct.GcObj** %14, align 4
  call void @"?SetPrev@GcRootList@@SAXPAUGcObj@@0@Z"(%struct.GcObj* %15, %struct.GcObj* %13)
  br label %16

; <label>:16:                                     ; preds = %12, %2
  %17 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %18 = getelementptr inbounds %class.GcList.0, %class.GcList.0* %5, i32 0, i32 0
  store %struct.GcObj* %17, %struct.GcObj** %18, align 4
  ret void
}

; Function Attrs: noinline optnone
define linkonce_odr dso_local x86_thiscallcc void @"?PrintList@?$GcList@VGcRootList@@@@QAEXXZ"(%class.GcList.0*) #0 comdat align 2 {
  %2 = alloca %class.GcList.0*, align 4
  %3 = alloca %struct.GcObj*, align 4
  %4 = alloca %struct.GcObj*, align 4
  store %class.GcList.0* %0, %class.GcList.0** %2, align 4
  %5 = load %class.GcList.0*, %class.GcList.0** %2, align 4
  %6 = getelementptr inbounds %class.GcList.0, %class.GcList.0* %5, i32 0, i32 0
  %7 = load %struct.GcObj*, %struct.GcObj** %6, align 4
  store %struct.GcObj* %7, %struct.GcObj** %3, align 4
  store %struct.GcObj* null, %struct.GcObj** %4, align 4
  %8 = call i8* @"?ListName@GcRootList@@SAPBDXZ"()
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @"??_C@_09EBJFJNG@?$CFs?5start?6?$AA@", i32 0, i32 0), i8* %8)
  br label %10

; <label>:10:                                     ; preds = %33, %1
  %11 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %12 = icmp ne %struct.GcObj* %11, null
  br i1 %12, label %13, label %37

; <label>:13:                                     ; preds = %10
  %14 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %15 = ptrtoint %struct.GcObj* %14 to i32
  %16 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @"??_C@_07FNDMELDM@obj?$DN?$CFd?6?$AA@", i32 0, i32 0), i32 %15)
  %17 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %18 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %17, i32 0, i32 4
  %19 = load i8, i8* %18, align 4
  %20 = trunc i8 %19 to i1
  %21 = zext i1 %20 to i32
  %22 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @"??_C@_0BD@HGGAKDEI@?5?5is_reachable?$DN?$CFd?6?$AA@", i32 0, i32 0), i32 %21)
  %23 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %24 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %23, i32 0, i32 5
  %25 = load i8*, i8** %24, align 4
  %26 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @"??_C@_0P@DPHCNOIJ@?5?5typename?$DN?$CFs?6?$AA@", i32 0, i32 0), i8* %25)
  %27 = load %struct.GcObj*, %struct.GcObj** %4, align 4
  %28 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %29 = call %struct.GcObj* @"?GetPrev@GcRootList@@SAPAUGcObj@@PAU2@@Z"(%struct.GcObj* %28)
  %30 = icmp ne %struct.GcObj* %27, %29
  br i1 %30, label %31, label %33

; <label>:31:                                     ; preds = %13
  %32 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([46 x i8], [46 x i8]* @"??_C@_0CO@HGIHIH@?5?5BADBADBADBADBADBADBADBADBADBAD@", i32 0, i32 0))
  br label %33

; <label>:33:                                     ; preds = %31, %13
  %34 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  store %struct.GcObj* %34, %struct.GcObj** %4, align 4
  %35 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %36 = call %struct.GcObj* @"?GetNext@GcRootList@@SAPAUGcObj@@PAU2@@Z"(%struct.GcObj* %35)
  store %struct.GcObj* %36, %struct.GcObj** %3, align 4
  br label %10

; <label>:37:                                     ; preds = %10
  %38 = call i8* @"?ListName@GcRootList@@SAPBDXZ"()
  %39 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @"??_C@_08BPFNJJP@?$CFs?5end?6?6?$AA@", i32 0, i32 0), i8* %38)
  ret void
}

; Function Attrs: noinline optnone
define dso_local void @gc_remove_root(%struct.GcObj*) #0 {
  %2 = alloca %struct.GcObj*, align 4
  store %struct.GcObj* %0, %struct.GcObj** %2, align 4
  %3 = load %struct.GcObj*, %struct.GcObj** %2, align 4
  call x86_thiscallcc void @"?Remove@?$GcList@VGcRootList@@@@QAEXPAUGcObj@@@Z"(%class.GcList.0* @"?gc_root_list@@3V?$GcList@VGcRootList@@@@A", %struct.GcObj* %3)
  call x86_thiscallcc void @"?PrintList@?$GcList@VGcRootList@@@@QAEXXZ"(%class.GcList.0* @"?gc_root_list@@3V?$GcList@VGcRootList@@@@A")
  ret void
}

; Function Attrs: noinline optnone
define linkonce_odr dso_local x86_thiscallcc void @"?Remove@?$GcList@VGcRootList@@@@QAEXPAUGcObj@@@Z"(%class.GcList.0*, %struct.GcObj*) #0 comdat align 2 {
  %3 = alloca %struct.GcObj*, align 4
  %4 = alloca %class.GcList.0*, align 4
  %5 = alloca %struct.GcObj*, align 4
  %6 = alloca %struct.GcObj*, align 4
  store %struct.GcObj* %1, %struct.GcObj** %3, align 4
  store %class.GcList.0* %0, %class.GcList.0** %4, align 4
  %7 = load %class.GcList.0*, %class.GcList.0** %4, align 4
  %8 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %9 = call %struct.GcObj* @"?GetNext@GcRootList@@SAPAUGcObj@@PAU2@@Z"(%struct.GcObj* %8)
  store %struct.GcObj* %9, %struct.GcObj** %5, align 4
  %10 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %11 = call %struct.GcObj* @"?GetPrev@GcRootList@@SAPAUGcObj@@PAU2@@Z"(%struct.GcObj* %10)
  store %struct.GcObj* %11, %struct.GcObj** %6, align 4
  %12 = load %struct.GcObj*, %struct.GcObj** %5, align 4
  %13 = icmp eq %struct.GcObj* %12, null
  br i1 %13, label %14, label %22

; <label>:14:                                     ; preds = %2
  %15 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %16 = getelementptr inbounds %class.GcList.0, %class.GcList.0* %7, i32 0, i32 0
  %17 = load %struct.GcObj*, %struct.GcObj** %16, align 4
  %18 = icmp ne %struct.GcObj* %15, %17
  br i1 %18, label %19, label %22

; <label>:19:                                     ; preds = %14
  %20 = call i8* @"?ListName@GcRootList@@SAPBDXZ"()
  %21 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([47 x i8], [47 x i8]* @"??_C@_0CP@HIJCJBBI@Tried?5to?5remove?5an?5obj?5that?8s?5no@", i32 0, i32 0), i8* %20)
  br label %43

; <label>:22:                                     ; preds = %14, %2
  %23 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %24 = getelementptr inbounds %class.GcList.0, %class.GcList.0* %7, i32 0, i32 0
  %25 = load %struct.GcObj*, %struct.GcObj** %24, align 4
  %26 = icmp eq %struct.GcObj* %23, %25
  br i1 %26, label %27, label %31

; <label>:27:                                     ; preds = %22
  %28 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %29 = call %struct.GcObj* @"?GetNext@GcRootList@@SAPAUGcObj@@PAU2@@Z"(%struct.GcObj* %28)
  %30 = getelementptr inbounds %class.GcList.0, %class.GcList.0* %7, i32 0, i32 0
  store %struct.GcObj* %29, %struct.GcObj** %30, align 4
  br label %31

; <label>:31:                                     ; preds = %27, %22
  %32 = load %struct.GcObj*, %struct.GcObj** %5, align 4
  %33 = icmp ne %struct.GcObj* %32, null
  br i1 %33, label %34, label %37

; <label>:34:                                     ; preds = %31
  %35 = load %struct.GcObj*, %struct.GcObj** %6, align 4
  %36 = load %struct.GcObj*, %struct.GcObj** %5, align 4
  call void @"?SetNext@GcRootList@@SAXPAUGcObj@@0@Z"(%struct.GcObj* %36, %struct.GcObj* %35)
  br label %37

; <label>:37:                                     ; preds = %34, %31
  %38 = load %struct.GcObj*, %struct.GcObj** %6, align 4
  %39 = icmp ne %struct.GcObj* %38, null
  br i1 %39, label %40, label %43

; <label>:40:                                     ; preds = %37
  %41 = load %struct.GcObj*, %struct.GcObj** %5, align 4
  %42 = load %struct.GcObj*, %struct.GcObj** %6, align 4
  call void @"?SetPrev@GcRootList@@SAXPAUGcObj@@0@Z"(%struct.GcObj* %42, %struct.GcObj* %41)
  br label %43

; <label>:43:                                     ; preds = %19, %40, %37
  ret void
}

declare dso_local i32 @printf(i8*, ...) #1

; Function Attrs: noinline nounwind optnone
define linkonce_odr dso_local i8* @"?ListName@GcObjList@@SAPBDXZ"() #2 comdat align 2 {
  ret i8* getelementptr inbounds ([8 x i8], [8 x i8]* @"??_C@_07DGBOGBKN@gc?5objs?$AA@", i32 0, i32 0)
}

; Function Attrs: noinline nounwind optnone
define linkonce_odr dso_local %struct.GcObj* @"?GetPrev@GcObjList@@SAPAUGcObj@@PAU2@@Z"(%struct.GcObj*) #2 comdat align 2 {
  %2 = alloca %struct.GcObj*, align 4
  store %struct.GcObj* %0, %struct.GcObj** %2, align 4
  %3 = load %struct.GcObj*, %struct.GcObj** %2, align 4
  %4 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %3, i32 0, i32 1
  %5 = load %struct.GcObj*, %struct.GcObj** %4, align 4
  ret %struct.GcObj* %5
}

; Function Attrs: noinline nounwind optnone
define linkonce_odr dso_local %struct.GcObj* @"?GetNext@GcObjList@@SAPAUGcObj@@PAU2@@Z"(%struct.GcObj*) #2 comdat align 2 {
  %2 = alloca %struct.GcObj*, align 4
  store %struct.GcObj* %0, %struct.GcObj** %2, align 4
  %3 = load %struct.GcObj*, %struct.GcObj** %2, align 4
  %4 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %3, i32 0, i32 0
  %5 = load %struct.GcObj*, %struct.GcObj** %4, align 4
  ret %struct.GcObj* %5
}

; Function Attrs: noinline nounwind optnone
define linkonce_odr dso_local void @"?SetNext@GcObjList@@SAXPAUGcObj@@0@Z"(%struct.GcObj*, %struct.GcObj*) #2 comdat align 2 {
  %3 = alloca %struct.GcObj*, align 4
  %4 = alloca %struct.GcObj*, align 4
  store %struct.GcObj* %1, %struct.GcObj** %3, align 4
  store %struct.GcObj* %0, %struct.GcObj** %4, align 4
  %5 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %6 = load %struct.GcObj*, %struct.GcObj** %4, align 4
  %7 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %6, i32 0, i32 0
  store %struct.GcObj* %5, %struct.GcObj** %7, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone
define linkonce_odr dso_local void @"?SetPrev@GcObjList@@SAXPAUGcObj@@0@Z"(%struct.GcObj*, %struct.GcObj*) #2 comdat align 2 {
  %3 = alloca %struct.GcObj*, align 4
  %4 = alloca %struct.GcObj*, align 4
  store %struct.GcObj* %1, %struct.GcObj** %3, align 4
  store %struct.GcObj* %0, %struct.GcObj** %4, align 4
  %5 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %6 = load %struct.GcObj*, %struct.GcObj** %4, align 4
  %7 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %6, i32 0, i32 1
  store %struct.GcObj* %5, %struct.GcObj** %7, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone
define linkonce_odr dso_local void @"?SetNext@GcRootList@@SAXPAUGcObj@@0@Z"(%struct.GcObj*, %struct.GcObj*) #2 comdat align 2 {
  %3 = alloca %struct.GcObj*, align 4
  %4 = alloca %struct.GcObj*, align 4
  store %struct.GcObj* %1, %struct.GcObj** %3, align 4
  store %struct.GcObj* %0, %struct.GcObj** %4, align 4
  %5 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %6 = load %struct.GcObj*, %struct.GcObj** %4, align 4
  %7 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %6, i32 0, i32 2
  store %struct.GcObj* %5, %struct.GcObj** %7, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone
define linkonce_odr dso_local void @"?SetPrev@GcRootList@@SAXPAUGcObj@@0@Z"(%struct.GcObj*, %struct.GcObj*) #2 comdat align 2 {
  %3 = alloca %struct.GcObj*, align 4
  %4 = alloca %struct.GcObj*, align 4
  store %struct.GcObj* %1, %struct.GcObj** %3, align 4
  store %struct.GcObj* %0, %struct.GcObj** %4, align 4
  %5 = load %struct.GcObj*, %struct.GcObj** %3, align 4
  %6 = load %struct.GcObj*, %struct.GcObj** %4, align 4
  %7 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %6, i32 0, i32 3
  store %struct.GcObj* %5, %struct.GcObj** %7, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone
define linkonce_odr dso_local i8* @"?ListName@GcRootList@@SAPBDXZ"() #2 comdat align 2 {
  ret i8* getelementptr inbounds ([9 x i8], [9 x i8]* @"??_C@_08NJAKNIEC@gc?5roots?$AA@", i32 0, i32 0)
}

; Function Attrs: noinline nounwind optnone
define linkonce_odr dso_local %struct.GcObj* @"?GetPrev@GcRootList@@SAPAUGcObj@@PAU2@@Z"(%struct.GcObj*) #2 comdat align 2 {
  %2 = alloca %struct.GcObj*, align 4
  store %struct.GcObj* %0, %struct.GcObj** %2, align 4
  %3 = load %struct.GcObj*, %struct.GcObj** %2, align 4
  %4 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %3, i32 0, i32 3
  %5 = load %struct.GcObj*, %struct.GcObj** %4, align 4
  ret %struct.GcObj* %5
}

; Function Attrs: noinline nounwind optnone
define linkonce_odr dso_local %struct.GcObj* @"?GetNext@GcRootList@@SAPAUGcObj@@PAU2@@Z"(%struct.GcObj*) #2 comdat align 2 {
  %2 = alloca %struct.GcObj*, align 4
  store %struct.GcObj* %0, %struct.GcObj** %2, align 4
  %3 = load %struct.GcObj*, %struct.GcObj** %2, align 4
  %4 = getelementptr inbounds %struct.GcObj, %struct.GcObj* %3, i32 0, i32 2
  %5 = load %struct.GcObj*, %struct.GcObj** %4, align 4
  ret %struct.GcObj* %5
}

attributes #0 = { noinline optnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { noinline nounwind optnone "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"NumRegisterParameters", i32 0}
!1 = !{i32 1, !"wchar_size", i32 2}
!2 = !{!"clang version 7.0.0 (tags/RELEASE_700/final)"}
