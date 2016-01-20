#!/bin/sh

header() {
cat > $output <<EOF
<?xml version='1.0'?>
<llcmcpp_cad xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespaceSchemaLocation="llcmcpp.xsd" >

<!-- CAD that make use of BRC/BRS to connect sequential instances of Client & Server -->

<ctypes>
  <ctype id="llcm_BasicBC_Vector_Redist_Client" dll="./llcm_BasicBC_Vector_Redist_Client.so" />
  <ctype id="llcm_BasicBC_Vector_Redist_Server" dll="./llcm_BasicBC_Vector_Redist_Server.so" />

  <ctype id="llcm_BasicBC_Vector_Client" dll="./llcm_BasicBC_Vector_Client.so" />
  <ctype id="llcm_BasicBC_Vector_Server" dll="./llcm_BasicBC_Vector_Server.so" />

  <ctype id="llcm_ParallelismManager" dll="./llcm_ParallelismManager.so" />
  <ctype id="llcm_BasicBC_Client" dll="./llcm_BasicBC.so" />
  <ctype id="llcm_BasicBC_Server" dll="./llcm_BasicBC.so" />
</ctypes>

EOF
}

footer() {
    cat >> $output <<EOF
</llcmcpp_cad>
EOF
}


gen_part_server() {
    # $1: id
    i=$1
    cat >> $output <<EOF
  <instance id="s$i" ctype="llcm_BasicBC_Vector_Server" />
  <instance id="sbrs$i" ctype="llcm_BasicBC_Vector_Redist_Server" />
  <instance id="spm$i" ctype="llcm_ParallelismManager" />
  <instance id="sbc$i" ctype="llcm_BasicBC_Server" />

EOF
}

gen_part_client() {
    # $1: id
    i=$1
    cat >> $output <<EOF
  <instance id="c$i" ctype="llcm_BasicBC_Vector_Client" />
  <instance id="cbr$i" ctype="llcm_BasicBC_Vector_Redist_Client" />
  <instance id="cbc$i" ctype="llcm_BasicBC_Client" />
  <instance id="cpm$i" ctype="llcm_ParallelismManager" />

EOF
}

gen_part_clients() {
    # $1 : #client
    i=0
    l=$1
    while [ $i -lt $l ]; do
	gen_part_client $i
	i=`expr $i + 1 `
    done
}

gen_part_servers() {
    # $1 : #servers
    i=0
    l=$1
    while [ $i -lt $l ]; do
	gen_part_server $i
	i=`expr $i + 1 `
    done
}

gen_partitioning() {
    # $1 : #client
    # $2 : #server
    cat >> $output <<EOF
<instances>
EOF

    gen_part_clients $1
    gen_part_servers $2

    cat >> $output <<EOF
</instances>

EOF
}

gen_cnx_server() {
    # $1 : id
    i=$1
    cat >> $output <<EOF
  <connection uid="s$i" uport="ubc" pid="sbc$i" pport="bc" />
  <connection uid="s$i" uport="upm" pid="spm$i" pport="pm" />

  <connection uid="sbrs$i" uport="upm" pid="spm$i" pport="pm" />
  <connection uid="sbrs$i" uport="ubc" pid="sbc$i" pport="bc" />
  <connection uid="sbrs$i" uport="udls" pid="sbc$i" pport="dls" />
  <connection uid="sbrs$i" uport="uva" pid="s$i" pport="va" />

EOF
}

gen_cnx_client() {
    # $1 : id
    i=$1
    cat >> $output <<EOF
  <connection uid="c$i" uport="ubc" pid="cbc$i" pport="bc" />
  <connection uid="c$i" uport="upm" pid="cpm$i" pport="pm" />
  <connection uid="c$i" uport="uva" pid="cbr$i" pport="va" />

  <connection uid="cbr$i" uport="upm" pid="cpm$i" pport="pm" />
  <connection uid="cbr$i" uport="ubc" pid="cbc$i" pport="bc" />
  <connection uid="cbr$i" uport="udlc" pid="cbc$i" pport="dlc" />

EOF
}

gen_cnx_clients_servers() {
    nc=$1
    ns=$2

    i=0
    # For each client
    while [ $i -lt $nc ]; do
	j=0
        # For each server
	while [ $j -lt $ns ]; do

	    cat >> $output <<EOF
  <connection uid="cbr$i" uport="uvai" pid="sbrs$j" pport="vai" />
EOF

	    j=`expr $j + 1 `
	done
	i=`expr $i + 1 `
    done


}

gen_cnx_clients() {
    # $1 : #client
    i=0
    l=$1
    while [ $i -lt $l ]; do
	gen_cnx_client $i
	i=`expr $i + 1 `
    done
}

gen_cnx_servers() {
    # $1 : #servers
    i=0
    l=$1
    while [ $i -lt $l ]; do
	gen_cnx_server $i
	i=`expr $i + 1 `
    done
}


gen_properties_pm () {
    # $1: prefix
    # $2 : $i (rank)
    # $3 : size

    cat >> $output <<EOF
  <value uid="$1$2" uport="rank" type="unsigned" value="$2" />
  <value uid="$1$2" uport="size" type="unsigned" value="$3" />
EOF
}

gen_properties_pms() {
    # $1: prefix
    # $2: n

    i=0
    l=$2
    while [ $i -lt $l ]; do
	gen_properties_pm $1 $i $2
	i=`expr $i + 1 `
    done
}

gen_properties_clients() {
    # $1: nc
    # $2: N
    # $3: BSZ

    i=0
    l=$1
    while [ $i -lt $l ]; do
	cat >> $output <<EOF
  <value uid="c$i" uport="N"    type="unsigned long" value="$2" />
  <value uid="c$i" uport="BSZ"  type="unsigned long" value="$3" />
EOF
	i=`expr $i + 1 `
    done
}

gen_properties_clients_all() {
    # $1: nc
    # $2: N
    # $3: BSZ

    gen_properties_clients $1 $2 $3
    gen_properties_pms cpm $1
}

gen_properties_server() {
    # $1: nc
    # $2: ns
    # $3: DUMP

    gen_properties_pms spm $2

    cat >> $output <<EOF

EOF

# useless becauser cbr retrieves info from _size (src) & vais.size (dst)
#     i=0
#     l=$1
#     while [ $i -lt $l ]; do
# 	cat >> $output <<EOF
#   <value uid="cbr$i" uport="srcTopo" type="unsigned" value="$1" />
#   <value uid="cbr$i" uport="dstTopo" type="unsigned" value="$2" />

# EOF
# 	i=`expr $i + 1 `
#     done

    i=0
    l=$2
    while [ $i -lt $l ]; do
	cat >> $output <<EOF
  <value uid="sbrs$i" uport="srcTopo" type="unsigned" value="$1" />
  <value uid="sbrs$i" uport="dstTopo" type="unsigned" value="$2" />

  <value uid="s$i" uport="DUMP" type="unsigned long" value="$3" />

EOF
	i=`expr $i + 1 `
    done

}


gen_connections() {
# $1 : #client
# $2 : #server
# $3 : data size
# $4 : bsz
# $5 : dump

    cat >> $output <<EOF
<connections>
EOF

    gen_cnx_clients $1
    gen_cnx_servers $2
    gen_cnx_clients_servers $1 $2

    cat >> $output <<EOF

EOF
    gen_properties_clients_all $1 $3 $4

    cat >> $output <<EOF

EOF
    gen_properties_server $1 $2 $5

    cat >> $output <<EOF
</connections>

EOF
}

gen_invocations() {
    # $1: nc
    # $2: ns

    cat >> $output <<EOF
<invocations>
EOF

    i=0
    l=$1
    while [ $i -lt $l ]; do
	cat >> $output <<EOF
  <invocation id="cbr$i" port="cc" type="ConfigureComplete" />
  <invocation id="c$i" port="cc" type="ConfigureComplete" />
EOF
	i=`expr $i + 1 `
    done

    i=0
    l=$2
    while [ $i -lt $l ]; do
	cat >> $output <<EOF
  <invocation id="sbrs$i" port="cc" type="ConfigureComplete" />
  <invocation id="s$i" port="cc" type="ConfigureComplete" />
EOF
	i=`expr $i + 1 `
    done

    i=0
    l=$1
    while [ $i -lt $l ]; do
	cat >> $output <<EOF
  <invocation id="c$i" port="go" type="Go" />
EOF
	i=`expr $i + 1 `
    done

    cat >> $output <<EOF
</invocations>
EOF
    
}   

gen_cad() {
    # $1: nc
    # $2: ns
    # $3: data size
    # $4: bsz
    # $5: dump

    header
    gen_partitioning $1 $2
    gen_connections $1 $2 $3 $4 $5
    gen_invocations $1 $2
    footer
}

######################
if [ ! $# -eq 6 ]; then
    echo "Usage: $0 <base> <nb_client> <nb_server> <data_size> <block size (0=bloc)> <dump: 0=no 1=yes>"
    exit -1
fi

base=$1
nc=$2
ns=$3
N=$4
BSZ=$5
DUMP=$6


output=$base.cad

gen_cad $nc $ns $N $BSZ $DUMP
