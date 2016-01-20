#!/bin/sh

# $1 = .f08 file with
# ! PORT_DECL $$ procedure, pass :: get_provide_##name => compo##_provide_##portname $$
# 
# & PORT_METHOD_DECL

SF=$1
DF=$2

DFT1="$DF"_tmp1
DFT2="$DF"_tmp2
DFT3="$DF"_tmp3
DFT4="$DF"_tmp4

function run_cpp()
{
    # cpp interprets Fortran // as comments :<
    sed 's/\/\//__DOUBLE_SLASH__/g' $1 | cpp -x c -E - | sed "s/@@/'/g" | sed 's/_LLCM_CR/\n/g' | sed 's/__DOUBLE_SLASH__/\/\//g' > $2
}

function component_port_method_generate() 
{
    if=$1
    of=$2

    # generating port decl
    grep PORT_DECL $if |  sed 's/.*PORT_DECL \$\$ \([^$]*\)\$\$.*/\1/g' | while read line 
    do
	cat $DFT1 | perl -npe "s/! PORT_METHOD_DECL/$line\n! PORT_METHOD_DECL/g" > $DFT2
	mv $DFT2 $DFT1
    done

    # cleaning
    mv $DFT1 $of
    if [ -e $DFT1 ]; then rm $DFT1; fi
    if [ -e $DFT2 ]; then rm $DFT2; fi
}

function prop_generate()
{
    # in/ou: DFT1
    cp $DFT1 $DFT3

    # generating component implementation
    grep PORT_PROP_IMPL $DFT1 | sed 's/.*PORT_PROP_IMPL\(.*\)/\1/g' | while read line 
    do
	cat $DFT3 | perl -npe "s/!_LLCMF_COMPONENT_IMPLEMENTATION/LLCMF_PROPERTY$line\n!_LLCMF_COMPONENT_IMPLEMENTATION/g" > $DFT2
	mv $DFT2 $DFT3
    done
    mv $DFT3 $DFT1
}

function component_generate()
{
    # in/ou: DFT1
    cp $DFT1 $DFT3

    # generating port prop implementation
    grep TYPE_COMPONENT_IMPL $DFT1 | sed 's/.*TYPE_COMPONENT_IMPL\(.*\)/\1/g' | while read line 
    do
	cat $DFT3 | perl -npe "s/!_LLCMF_COMPONENT_IMPLEMENTATION/LLCMF$line\n!_LLCMF_COMPONENT_IMPLEMENTATION/g" > $DFT2
	mv $DFT2 $DFT3
    done
    mv $DFT3 $DFT1
}

function provide_generate()
{
    # in/ou: DFT1
    cp $DFT1 $DFT3

    # generating component implementation
    grep PORT_PROVIDE_IMPL $DFT1 | sed 's/.*PORT_PROVIDE_IMPL\(.*\)/\1/g' | while read line 
    do
	cat $DFT3 | perl -npe "s/!_LLCMF_COMPONENT_IMPLEMENTATION/LLCMF_PROVIDE$line\n!_LLCMF_COMPONENT_IMPLEMENTATION/g" > $DFT2
	mv $DFT2 $DFT3
    done
    mv $DFT3 $DFT1
}

function use_generate()
{
    # in/ou: DFT1
    cp $DFT1 $DFT3

    # generating component implementation
    grep PORT_USE_IMPL $DFT1 | sed 's/.*PORT_USE_IMPL\(.*\)/\1/g' | while read line 
    do
	cat $DFT3 | perl -npe "s/!_LLCMF_COMPONENT_IMPLEMENTATION/LLCMF_USE$line\n!_LLCMF_COMPONENT_IMPLEMENTATION/g" > $DFT2
	mv $DFT2 $DFT3
    done
    mv $DFT3 $DFT1
}

function implementation_generate()
{
    if=$1
    of=$2

    mv $if $DFT1

    # generating impl macros
    component_generate
    provide_generate
    use_generate
    prop_generate

    # cleaning
    cat $DFT1 | perl -npe "s/!_LLCMF_COMPONENT_IMPLEMENTATION//g" > $DFT2


    # adding include
    echo "#include <llcmcpp/llcm.f>" > $DFT1
    cat $DFT2 >> $DFT1

    # expansing impl macros (2nd time)   
    run_cpp $DFT1 $of
}


# expansing macros (1st time)
run_cpp $SF $DFT1
component_port_method_generate $DFT1 $DFT4
implementation_generate $DFT4 $DF

# cleaning
if [ -e $DFT1 ]; then rm $DFT1; fi
if [ -e $DFT2 ]; then rm $DFT2; fi
if [ -e $DFT3 ]; then rm $DFT3; fi
if [ -e $DFT4 ]; then rm $DFT4; fi
