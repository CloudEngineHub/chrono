// #######################################################
// MACRO FOR AUTOMATIC CASTING of raw pointers
//
// A basic mechanism to provide downcasting and upcasting between base pointers and specialized types,
// when polymorphic objects return from a function.
// Parameters: base class, inherited, inherited, inherited, ...


// This works, but results in code bloat...

%define %_classes_dispatch(Type) 
	if ( typeid(*$1)==typeid(Type) )
		return (SWIG_NewPointerObj(%as_voidptr($1),$descriptor(Type *), $owner | %newpointer_flags));  
%enddef

%define %downcast_output(OutType,Types...)
%typemap(out) OutType*, OutType& {
  %formacro(%_classes_dispatch, Types)
    return(SWIG_NewPointerObj(%as_voidptr($1),$descriptor(OutType *), $owner | %newpointer_flags));
}
%enddef


// #######################################################
// MACRO FOR GENERIC CASTING of std::shared_ptr
// 
// This enables the manual CASTING (usually from base to derived,
// but could be also viceversa) by calling a python function.
// For example:  myvis = CastToChVisualizationShared(myasset)

%define %DefSharedPtrDynamicCast(__NS__,__CHTYPE_BASE__, __CHTYPE__)
%inline %{
  std::shared_ptr<__NS__::__CHTYPE__> CastTo ## __CHTYPE__ ##(std::shared_ptr<__NS__::__CHTYPE_BASE__> in_obj) 
  {
	  return (std::dynamic_pointer_cast<__NS__::__CHTYPE__>(in_obj));
  }
%}
%enddef

%define %DefSharedPtrDynamicCast2NS(__NS1__,__NS2__,__CHTYPE_BASE__, __CHTYPE__)
%inline %{
  std::shared_ptr<__NS2__::__CHTYPE__> CastTo ## __CHTYPE__ ##(std::shared_ptr<__NS1__::__CHTYPE_BASE__> in_obj) 
  {
	  return (std::dynamic_pointer_cast<__NS2__::__CHTYPE__>(in_obj));
  }
%}
%enddef

// Same as DefSharedPtrDynamicCast, but the third parameter is the typename in a 
// readable form. For example if __CHTYPE__ is a templated class such as Myclass<Mytype>, 
// DefSharedPtrDynamicCast cannot be used because it would attempt to generate 
// a function "CastToMyclass<Mytype>" that cannot be wrapped because of the <> brackets. Same for :: namespaces etc. 
// So here one could use for example  DefSharedPtrDynamicCastCustomName(Mybase, Myclass<Mytype>, Myclass_Tmytype)
// to generate "CastToMyclass_Tmytype".
%define %DefSharedPtrDynamicCastCustomName(__NS1__,__NS2__,__CHTYPE_BASE__, __CHTYPE__, __CHTYPEREADABLE__)
%inline %{
	std::shared_ptr<__CHTYPE__> CastTo ## __CHTYPEREADABLE__ ##(std::shared_ptr<__CHTYPE_BASE__> in_obj)
	{
		return (std::dynamic_pointer_cast<__CHTYPE__>(in_obj));
	}
%}
%enddef




// #######################################################
// MACRO FOR AUTOMATIC CASTING of std::shared_ptr
// 
// Utility macro for enabling AUTOMATIC casting function  outputs with std::shared_ptr pointers. 
// Since this creates lot of code bloat, it is suggested to use it sparingly; prefer using the 
// DefSharedPtrDynamicCast macro when possible.
//
// Parameters: base class, inherited, inherited, ... (only the classes wrapped by the shared ptr)


%define %_shpointers_dispatch(Type) 
	if ( std::dynamic_pointer_cast<Type>(result) )
		return(SWIG_NewPointerObj((new std::shared_ptr<Type>(std::dynamic_pointer_cast<Type>(result) )), $descriptor(std::shared_ptr<Type> *), SWIG_POINTER_OWN |  0 ));
%enddef

%define %_shpointers_dispatchR(Type) 
	if ( std::dynamic_pointer_cast<Type>(*result) )
		return(SWIG_NewPointerObj((new std::shared_ptr<Type>(std::dynamic_pointer_cast<Type>(*result) )), $descriptor(std::shared_ptr<Type> *), SWIG_POINTER_OWN |  0 ));
%enddef

%define %_shpointers_dispatchP(Type) 
	if ( std::dynamic_pointer_cast<Type>(*result) )
		return(SWIG_NewPointerObj((new std::shared_ptr<Type>(std::dynamic_pointer_cast<Type>(*result) )), $descriptor(std::shared_ptr<Type> *), SWIG_POINTER_OWN |  0 ));
%enddef


//***THE MACRO***
%define %downcast_output_sharedptr(OutType,Types...)
%typemap(out) std::shared_ptr<OutType> {
  %formacro(%_shpointers_dispatch, Types)
    return(SWIG_NewPointerObj((new std::shared_ptr<OutType>(std::static_pointer_cast<OutType>(result) )), $descriptor(std::shared_ptr<OutType> *), SWIG_POINTER_OWN |  0 ));
}
%typemap(out) std::shared_ptr<OutType>& {
  %formacro(%_shpointers_dispatchR, Types)
    return(SWIG_NewPointerObj ((new std::shared_ptr<OutType>(std::static_pointer_cast<OutType>(*result) )), $descriptor(std::shared_ptr<OutType> *), SWIG_POINTER_OWN |  0 ));
}
%typemap(out) std::shared_ptr<OutType>* {
	if ($owner) delete $1;
  %formacro(%_shpointers_dispatchR, Types)
    return(SWIG_NewPointerObj  ((new std::shared_ptr<OutType>(std::static_pointer_cast<OutType>(*result) )), $descriptor(std::shared_ptr<OutType> *), SWIG_POINTER_OWN |  0 ));
}
%enddef
