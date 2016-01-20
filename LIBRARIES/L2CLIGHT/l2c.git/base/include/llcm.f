!
!    This file is part of LLCMc++ (a.k.a. llcmcpp)
!    Copyright (C) 2011 INRIA Julien Bigot <julien.bigot@inria.fr>
!    Copyright (C) 2011 INRIA Christian Pérez <christian.perez@inria.fr>
!
!    LLCMc++ is free software: you can redistribute it and/or modify
!    it under the terms of the GNU Lesser General Public License as published by
!    the Free Software Foundation, either version 3 of the License, or
!    (at your option) any later version.
!
!    LLCMc++ is distributed in the hope that it will be useful,
!    but WITHOUT ANY WARRANTY; without even the implied warranty of
!    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
!    GNU Lesser General Public License for more details.
!
!    You should have received a copy of the GNU Lesser General Public License
!    along with LLCMc++.  If not, see <http://www.gnu.org/licenses/>.
!

#ifndef LLCMCPP_LLCM_F
#define LLCMCPP_LLCM_F

#define LCMP(compo) \
  type, extends(Component_t) :: compo _LLCM_CR\
! TYPE_COMPONENT_IMPL (compo)

#define L_PROPERTY(compo, proptype, propname, Ctype, conversion)   proptype propname _LLCM_CR\
! PORT_DECL $$ procedure, pass :: set_prop_##propname => compo##_set_##propname $$_LLCM_CR\
! PORT_PROP_IMPL(compo, proptype, propname, Ctype, conversion)

#define L_F_PROVIDE(compo, porttype, portname)   type(porttype) portname _LLCM_CR\
! PORT_DECL $$ procedure, pass :: get_provide_##portname => compo##_provide_##portname $$ _LLCM_CR\
! PORT_PROVIDE_IMPL(compo, porttype, portname)

#define L_F_USE(compo, porttype, portname)   type(porttype), pointer :: portname _LLCM_CR\
! PORT_DECL $$ procedure, pass :: set_use_##portname => compo##_set_##portname $$ _LLCM_CR\
! PORT_USE_IMPL(compo, porttype, portname)

#define LEND(compo) \
!   PORT_METHOD_DECL _LLCM_CR\
   end type compo _LLCM_CR

#define LLCMF(compo)\
  subroutine compo##_c_create(cp) BIND(C, NAME=@@compo##_c_create@@) _LLCM_CR\
    use iso_c_binding _LLCM_CR\
    implicit none _LLCM_CR\
    type(c_ptr), intent(out) :: cp _LLCM_CR\
    type (compo), pointer :: this _LLCM_CR\
    call compo##_create(this) _LLCM_CR\
    cp = c_loc(this) _LLCM_CR\
  end subroutine compo##_c_create_LLCM_CR\
_LLCM_CR\
  subroutine compo##_c_destroy(cp) BIND(C, NAME=@@compo##_c_destroy@@)_LLCM_CR\
    use iso_c_binding_LLCM_CR\
    implicit none_LLCM_CR\
    type(c_ptr), intent(in) :: cp_LLCM_CR\
    type (compo), pointer :: this_LLCM_CR\
    call c_f_pointer(cp, this)_LLCM_CR\
    call compo##_destroy(this)_LLCM_CR\
  end subroutine compo##_c_destroy

#define LLCMF_PROVIDE(compo, porttype, portname)_LLCM_CR\
  subroutine compo##_provide_##portname(this, p)_LLCM_CR\
    class (compo), target, intent(in) :: this_LLCM_CR\
    type (porttype), pointer, intent(out) :: p_LLCM_CR\
    p=>this%portname _LLCM_CR\
  end subroutine compo##_provide_##portname _LLCM_CR\
\
  subroutine compo##_c_get_##portname(cthis, cport) BIND(C, NAME=@@compo##_c_get_##portname@@)_LLCM_CR\
    use iso_c_binding_LLCM_CR\
    implicit none_LLCM_CR\
    type(c_ptr), intent(in) :: cthis_LLCM_CR\
    type(c_ptr), intent(inout) :: cport_LLCM_CR\
    type (compo), pointer :: this_LLCM_CR\
    type (porttype), pointer :: port_LLCM_CR\
    call c_f_pointer(cthis, this)_LLCM_CR\
    call this%get_provide_##portname(port)_LLCM_CR\
    cport =c_loc(port)_LLCM_CR\
  end subroutine compo##_c_get_##portname _LLCM_CR

#define LLCMF_C_PROVIDE_SUBROUTINE(compo, port, subname) _LLCM_CR\
  subroutine compo##_c_##subname(cp) BIND(C, NAME=@@compo##_c_##subname@@)_LLCM_CR\
    use iso_c_binding_LLCM_CR\
    implicit none_LLCM_CR\
    type(c_ptr), intent(in) :: cp_LLCM_CR\
    type (compo), pointer :: this_LLCM_CR\
    call c_f_pointer(cp, this)_LLCM_CR\
    call this%port%subname()_LLCM_CR\
  end subroutine compo##_C_##subname##_LLCM_CR\

#define LLCMF_USE(compo, porttype, portname)_LLCM_CR\
  subroutine compo##_set_##portname(this, port)_LLCM_CR\
    class (compo), target, intent(inout) :: this_LLCM_CR\
    type (porttype), pointer, intent(in) :: port_LLCM_CR\
    this%portname => port_LLCM_CR\
  end subroutine compo##_set_##portname _LLCM_CR\
\
  subroutine compo##_c_set_##portname(cthis, cport) BIND(C, NAME=@@compo##_c_set_##portname@@)_LLCM_CR\
    use iso_c_binding_LLCM_CR\
    implicit none_LLCM_CR\
    type(c_ptr), intent(in) :: cthis_LLCM_CR\
    type (compo), pointer :: this_LLCM_CR\
    type (c_ptr), intent(in) :: cport_LLCM_CR\
    type (porttype), pointer :: port_LLCM_CR\
    call c_f_pointer(cthis, this)_LLCM_CR\
    call c_f_pointer(cport, port)_LLCM_CR\
    call this%set_use_##portname(port)_LLCM_CR\
  end subroutine compo##_c_set_##portname _LLCM_CR\

#define LLCMF_PROPERTY(compo, Ftype, portname, Ctype, conversion)_LLCM_CR\
  subroutine compo##_set_##portname(this, val)_LLCM_CR\
    class (compo), target, intent(inout) :: this_LLCM_CR\
    Ftype, intent(in) :: val_LLCM_CR\
    this%portname=val_LLCM_CR\
  end subroutine compo##_set_##portname _LLCM_CR\
_LLCM_CR\
  subroutine compo##_c_set_##portname(cthis, iter) BIND(C, NAME=@@compo##_c_set_##portname@@)_LLCM_CR\
    use iso_c_binding_LLCM_CR\
    implicit none_LLCM_CR\
    type(c_ptr), intent(in) :: cthis_LLCM_CR\
    type (compo), pointer :: this_LLCM_CR\
    integer(KIND=Ctype) iter_LLCM_CR\
    call c_f_pointer(cthis, this)_LLCM_CR\
    call this%set_prop_##portname(conversion(iter))_LLCM_CR\
  end subroutine compo##_c_set_##portname _LLCM_CR

#define LLCMF_COMPONENT_IMPLEMENTATION !_LLCMF_COMPONENT_IMPLEMENTATION

#endif // LLCMCPP_LLCM_F
