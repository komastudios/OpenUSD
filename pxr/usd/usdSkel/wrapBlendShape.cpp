//
// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#include "pxr/usd/usdSkel/blendShape.h"
#include "pxr/usd/usd/schemaBase.h"

#include "pxr/usd/sdf/primSpec.h"

#include "pxr/usd/usd/pyConversions.h"
#include "pxr/base/tf/pyContainerConversions.h"
#include "pxr/base/tf/pyResultConversions.h"
#include "pxr/base/tf/pyUtils.h"
#include "pxr/base/tf/wrapTypeHelpers.h"

#include <boost/python.hpp>

#include <string>

using namespace boost::python;

PXR_NAMESPACE_USING_DIRECTIVE

namespace {

#define WRAP_CUSTOM                                                     \
    template <class Cls> static void _CustomWrapCode(Cls &_class)

// fwd decl.
WRAP_CUSTOM;

        
static UsdAttribute
_CreateOffsetsAttr(UsdSkelBlendShape &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateOffsetsAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Vector3fArray), writeSparsely);
}
        
static UsdAttribute
_CreateNormalOffsetsAttr(UsdSkelBlendShape &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreateNormalOffsetsAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->Vector3fArray), writeSparsely);
}
        
static UsdAttribute
_CreatePointIndicesAttr(UsdSkelBlendShape &self,
                                      object defaultVal, bool writeSparsely) {
    return self.CreatePointIndicesAttr(
        UsdPythonToSdfType(defaultVal, SdfValueTypeNames->IntArray), writeSparsely);
}

static std::string
_Repr(const UsdSkelBlendShape &self)
{
    std::string primRepr = TfPyRepr(self.GetPrim());
    return TfStringPrintf(
        "UsdSkel.BlendShape(%s)",
        primRepr.c_str());
}

} // anonymous namespace

void wrapUsdSkelBlendShape()
{
    typedef UsdSkelBlendShape This;

    class_<This, bases<UsdTyped> >
        cls("BlendShape");

    cls
        .def(init<UsdPrim>(arg("prim")))
        .def(init<UsdSchemaBase const&>(arg("schemaObj")))
        .def(TfTypePythonClass())

        .def("Get", &This::Get, (arg("stage"), arg("path")))
        .staticmethod("Get")

        .def("Define", &This::Define, (arg("stage"), arg("path")))
        .staticmethod("Define")

        .def("GetSchemaAttributeNames",
             &This::GetSchemaAttributeNames,
             arg("includeInherited")=true,
             return_value_policy<TfPySequenceToList>())
        .staticmethod("GetSchemaAttributeNames")

        .def("_GetStaticTfType", (TfType const &(*)()) TfType::Find<This>,
             return_value_policy<return_by_value>())
        .staticmethod("_GetStaticTfType")

        .def(!self)

        
        .def("GetOffsetsAttr",
             &This::GetOffsetsAttr)
        .def("CreateOffsetsAttr",
             &_CreateOffsetsAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetNormalOffsetsAttr",
             &This::GetNormalOffsetsAttr)
        .def("CreateNormalOffsetsAttr",
             &_CreateNormalOffsetsAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))
        
        .def("GetPointIndicesAttr",
             &This::GetPointIndicesAttr)
        .def("CreatePointIndicesAttr",
             &_CreatePointIndicesAttr,
             (arg("defaultValue")=object(),
              arg("writeSparsely")=false))

        .def("__repr__", ::_Repr)
    ;

    _CustomWrapCode(cls);
}

// ===================================================================== //
// Feel free to add custom code below this line, it will be preserved by 
// the code generator.  The entry point for your custom code should look
// minimally like the following:
//
// WRAP_CUSTOM {
//     _class
//         .def("MyCustomMethod", ...)
//     ;
// }
//
// Of course any other ancillary or support code may be provided.
// 
// Just remember to wrap code in the appropriate delimiters:
// 'namespace {', '}'.
//
// ===================================================================== //
// --(BEGIN CUSTOM CODE)--

namespace {


tuple
_ValidatePointIndices(TfSpan<const int> pointIndices,
                      size_t numPoints)
{
    std::string reason;
    bool valid = UsdSkelBlendShape::ValidatePointIndices(
        pointIndices, numPoints, &reason);
    return boost::python::make_tuple(valid, reason);
}


WRAP_CUSTOM {

    using This = UsdSkelBlendShape;

    _class
        .def("CreateInbetween", &This::CreateInbetween, arg("name"))
        .def("GetInbetween", &This::GetInbetween, arg("name"))
        .def("HasInbetween", &This::HasInbetween, arg("name"))
        
        .def("GetInbetweens", &This::GetInbetweens,
             return_value_policy<TfPySequenceToList>())
        .def("GetAuthoredInbetweens", &This::GetAuthoredInbetweens,
             return_value_policy<TfPySequenceToList>())

        .def("ValidatePointIndices", &_ValidatePointIndices,
             (arg("pointIndices"), arg("numPoints")))
        .staticmethod("ValidatePointIndices")
        ;
}

}
