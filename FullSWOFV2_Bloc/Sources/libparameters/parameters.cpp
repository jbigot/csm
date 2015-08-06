#include "parser.hpp"
#include "parameters.hpp"
#include <unistd.h>

parameters::parameters() {}

//-------------------
parameters * parameters::singleton=NULL;
parameters * parameters::getSingleton() {
    if(singleton==NULL)
        singleton =  new parameters();
    return singleton;
}
//-------------------

///////////////////////////////////////////////
// A constructor that gets all the parameters
// from the file "filename" and save them
///////////////////////////////////////////////

void parameters::setparameters(const char * filename) {

    parser fileParser(filename);

    string path_input_directory ("./Inputs/");
    string path_output_directory ("./Outputs");

    Nxcell = atoi(fileParser.GetValue("<Nxcell>").c_str());
    Nycell = atoi(fileParser.GetValue("<Nycell>").c_str());
    dx = atof(fileParser.GetValue("<spacing>").c_str());
    dy = dx;

    T = atof(fileParser.GetValue("<T>").c_str());
    cfl_fix = atof(fileParser.GetValue("<cflfix>").c_str());
    dt_fix = atof(fileParser.GetValue("<dtfix>").c_str());
    nsave = atoi(fileParser.GetValue("<nsave>").c_str());

    Lbound = atoi(fileParser.GetValue("<Lbound>").c_str());
    left_imp_discharge =  atof(fileParser.GetValue("left_imp_discharge").c_str());
    left_imp_h =  atof(fileParser.GetValue("left_imp_h").c_str());
    Rbound = atoi(fileParser.GetValue("<Rbound>").c_str());
    right_imp_discharge =  atof(fileParser.GetValue("right_imp_discharge").c_str());
    right_imp_h =  atof(fileParser.GetValue("right_imp_h").c_str());
    Bbound = atoi(fileParser.GetValue("<Bbound>").c_str());
    bottom_imp_discharge =  atof(fileParser.GetValue("bottom_imp_discharge").c_str());
    bottom_imp_h =  atof(fileParser.GetValue("bottom_imp_h").c_str());
    Tbound = atoi(fileParser.GetValue("<Tbound>").c_str());
    top_imp_discharge =  atof(fileParser.GetValue("top_imp_discharge").c_str());
    top_imp_h =  atof(fileParser.GetValue("top_imp_h").c_str());

    order = atoi(fileParser.GetValue("<order>").c_str());

    flux = atoi(fileParser.GetValue("<flux>").c_str());

    rec = atoi(fileParser.GetValue("<rec>").c_str());
    lim = atoi(fileParser.GetValue("<lim>").c_str());
    amortENO = atof(fileParser.GetValue("<amortENO>").c_str());
    modifENO = atof(fileParser.GetValue("<modifENO>").c_str());

    fric = atoi(fileParser.GetValue("<fric>").c_str());
    fric_init = atoi(fileParser.GetValue("<fricinit>").c_str());
    fric_namefile_short = fileParser.GetValue("<fric_NF>");
    fric_namefile = path_input_directory + fric_namefile_short;
    if (fric_init==1 && access (fric_namefile.c_str(), R_OK) == -1 ) {
        cout << " the friction file "<< fric_namefile_short<< " does not exists in the directory Inputs" << endl ;
        exit(-1);
    } else {
        fric_coef = atof(fileParser.GetValue("<friccoef>").c_str());
    }

    inf = atoi(fileParser.GetValue("<inf>").c_str());

    Ks_init = atoi(fileParser.GetValue("<Ks_init>").c_str());
    Ks_namefile_short = fileParser.GetValue("<Ks_NF>");
    Ks_namefile = path_input_directory + Ks_namefile_short;
    if (Ks_init==1 && access (Ks_namefile.c_str(), R_OK) == -1 ) {
        cout << " the  file "<< Ks_namefile_short<< " does not exists in the directory Inputs" << endl ;
        exit(-1);
    } else {
        Ks_coef = atof(fileParser.GetValue("<Kscoef>").c_str());
    }

    dtheta_init = atoi(fileParser.GetValue("<dtheta_init>").c_str());
    dtheta_namefile_short = fileParser.GetValue("<dtheta_NF>");
    dtheta_namefile = path_input_directory + dtheta_namefile_short;
    if (dtheta_init==1 && access (dtheta_namefile.c_str(), R_OK) == -1 ) {
        cout << " the  file "<< dtheta_namefile_short<< " does not exists in the directory Inputs" << endl ;
        exit(-1);
    } else {
        dtheta_coef = atof(fileParser.GetValue("<dthetacoef>").c_str());
    }

    Psi_init = atoi(fileParser.GetValue("<Psi_init>").c_str());
    Psi_namefile_short = fileParser.GetValue("<Psi_NF>");
    Psi_namefile = path_input_directory + Psi_namefile_short;
    if (Psi_init==1 && access (Psi_namefile.c_str(), R_OK) == -1 ) {
        cout << " the  file "<< Psi_namefile_short<< " does not exists in the directory Inputs" << endl ;
        exit(-1);
    } else {
        Psi_coef = atof(fileParser.GetValue("<Psicoef>").c_str());
    }

    retension_init = atoi(fileParser.GetValue("<retension_init>").c_str());
    retension_namefile_short = fileParser.GetValue("<retension_NF>");
    retension_namefile = path_input_directory + retension_namefile_short;
    if (retension_init==1 && access (retension_namefile.c_str(), R_OK) == -1 ) {
        cout << " the  file "<< retension_namefile_short<< " does not exists in the directory Inputs" << endl ;
        exit(-1);
    } else {
        retension_coef = atof(fileParser.GetValue("<retensioncoef>").c_str());
    }

    topo = atoi(fileParser.GetValue("<topo>").c_str());
    topography_namefile_short = fileParser.GetValue("<topo_NF>");
    topography_namefile = path_input_directory + topography_namefile_short;
    if (topo==1 && access (topography_namefile.c_str(), R_OK) == -1 ) {
        cout << " the topography file "<< topography_namefile_short<< " does not exists in the directory Inputs" << endl ;
        exit(-1);
    }

    h_initialization = atoi(fileParser.GetValue("<h_init>").c_str());
    h_namefile_short = fileParser.GetValue("<h_NF>");
    h_namefile = path_input_directory + h_namefile_short;
    if (h_initialization ==1 && access (h_namefile.c_str(), R_OK) == -1 ) {
        cout << " the huv file "<< h_namefile_short << " does not exists in the directory Inputs" << endl ;
        exit(-1);
    }

    rain = atoi(fileParser.GetValue("<rain>").c_str());
    rain_namefile_short = fileParser.GetValue("<rain_NF>");
    rain_namefile = path_input_directory + rain_namefile_short;
    if (rain==1 &&  access (rain_namefile.c_str(), R_OK) == -1) {
        cout << " the rain file "<< rain_namefile_short << " does not exists in the directory Inputs" << endl ;
        exit(-1);
    }

    suffix_outputs = fileParser.GetValue("<suffix_o>");
    output_format = atoi(fileParser.GetValue("<output_f>").c_str());
    output_directory = path_output_directory + suffix_outputs + "/";
    /*

    /////////////////////////////////////////////////////////////////////////////////////////////
        namefile = output_directory + "parameters.dat";
        ofstream param(namefile.c_str(),ios::out);
        if (!param) {
            cout << "Impossible to open the " << namefile.c_str() <<" file\n";
            cout << "Verify if the directory " << output_directory <<" exists\n";
            exit(-1);
        }


        param << "#####################################################################" << endl;
        param << "# Input parameters" << endl;
        param << "#####################################################################" << endl;
        param << endl;
        param << "Number of meshes (x-axis)  <Nxcell>:: "<< Nxcell << endl;
        param << "Number of meshes (y-axis)  <Nycell>:: "<< Nycell << endl;
        param << "Cell size (dx=dy in meter)  <spacing>:: "<< dx << endl;
        param << endl;
        param << "Time of simulation (in seconds) <T>:: "<< T << endl;
        param << "Timestep (in seconds) <dtfix>:: "<< dt_fix << endl;
        param << "cflfix, value of the cfl  <cflfix>:: "<< cfl_fix << endl;
        param << "Number of interations for each save  <nsave>:: "<< nsave << endl;
        param << endl;
        param << "Left Boundary condition   (x = 0)   (1=wall 2=neumann 3=liquid 4=imp_discharge)  <Lbound>:: "<< Lbound << endl;
        param << "Imposed discharge in left bc <left_imp_discharge> :: "<< left_imp_discharge << endl;
        param << "Imposed height in left bc (if flow super critical) <left_imp_h>:: "<< left_imp_h << endl;
        param << endl;
        param << "Right Boundary condition (x = xmax)(1=wall 2=neumann 3=liquid 4=imp_discharge)  <Rbound>:: "<< Rbound << endl;
        param << "Imposed discharge in right bc <right_imp_discharge> :: "<< right_imp_discharge << endl;
        param << "Imposed height in right bc (if flow super criti cal) <right_imp_h>:: "<< right_imp_h << endl;
        param << endl;
        param << "Bottom Boundary condition (y = 0)   (1=wall 2=neumann 3=liquid 4=imp_discharge)  <Bbound>:: "<< Bbound << endl;
        param << "Imposed discharge in bottom  bc <bottom_imp_discharge> :: "<< bottom_imp_discharge << endl;
        param << "Imposed height in bottom bc (if flow super critical) <bottom_imp_h>:: "<< bottom_imp_h << endl;
        param << endl;
        param << "Top Boundary condition bc (y = ymax)(1=wall 2=neumann 3=liquid 4=imp_discharge)  <Tbound>:: "<< Tbound << endl;
        param << "Imposed discharge in top bc <top_imp_discharge> :: "<< top_imp_discharge << endl;
        param << "Imposed height in top bc (if flow super critical) <top_imp_h>:: "<< top_imp_h << endl;
        param << endl;
        param << "Order of the scheme (1=order1 2=order2)  <order>:: "<< order << endl;
        param << endl;
        param << "Numerical flux (1=Rusanov 2=HLL 3=HLL2)  <flux>:: "<< flux << endl;
        param << endl;
        param << "Reconstruction (1=MUSCL 2=ENO 3=ENOmod)  <rec>:: "<< rec << endl;
        param << "Limiter (1=Minmod 2=VanAlbada 3=VanLeer)  <lim>:: "<< lim << endl;
        param << "AmortENO (Between 0 and 1.)  <amortENO>:: "<< amortENO << endl;
        param << "ModifENO (Between 0 and 1.)  <modifENO>:: "<< modifENO << endl;
        param << endl;
        param << "Friction law (1=Manning 2=Darcy-Weisbach)  <fric>:: "<< fric << endl;
        param << "Initialisation of Friction (1=file 2=const_coef) <fricinit>:: "<< fric_init << endl;
        param << "Friction coefficient  <friccoef>:: "<< fric_coef << endl;
        param << "Name of the friction file  <fric_NF>:: "<< fric_namefile_short << endl;
        param << endl;
        param << "Infiltration model (1=Green-Ampt 2=Without infiltration)  <inf>:: "<< inf << endl;
        param << endl;
        param << "Ks, hydraulic conductivity (saturation) of the surface (1=file 2=const_coef) <Ks_init>:: "<< Ks_init << endl;
        param << "Ks coefficient  (1.7e-8)  <Kscoef>:: "<< Ks_coef << endl;
        param << "Name of the Ks file <Ks_NF>:: "<< Ks_namefile_short << endl;
        param << endl;
        param << "Dteta, water content  (1=file 2=const_coef) <dtheta_init>:: "<< dtheta_init << endl;
        param << "dtheta coefficient  <dthetacoef>:: "<< dtheta_coef << endl;
        param << "Name of the dtheta file <dtheta_NF>:: "<< dtheta_namefile_short << endl;
        param << endl;
        param << "Psi, load pressure (1=file 2=const_coef)  <Psi_init>:: "<< Psi_init << endl;
        param << "Psi coefficient   <Psicoef>:: "<< Psi_coef << endl;
        param << "Name of the Psi file <Psi_NF>:: "<< Psi_namefile_short << endl;
        param << endl;
        param << "Thresold of retension (1=file 2=const_coef)  <retension_init>:: "<< retension_init << endl;
        param << "Retension coefficient   <retensioncoef>:: "<< retension_coef << endl;
        param << "Name of the retension file <retension_NF>:: "<< retension_namefile_short << endl;
        param << endl;
        param << "Topography (1=file 2=flat)  <topo>:: "<< topo << endl;
        param << "Name of the topography file  <topo_NF>:: "<< topography_namefile_short << endl;
        param << endl;
        param << "Initialization of h, u and v (1=file 2:h=0)  <h_init>:: "<< h_initialization << endl;
        param << "Name of the h initialization file  <h_NF>:: "<< h_namefile_short << endl;
        param << endl;
        param << "Rain (1=file 2=function)  <rain>:: "<< rain << endl;
        param << "Name of the rain file  <rain_NF>:: "<< rain_namefile_short << endl;
        param << endl;
        param << "Suffix for the 'Outputs' directory  <suffix_o>:: "<< suffix_outputs << endl;
        param << "Format of the Output file (1=gnuplot 2=vtk)  <output_f>:: "<< output_format << endl;
        param.close();

     //   cout << "------------------------------------------------------------------------" << endl;
     //   cout << "entries ok" << endl;
     //   cout << "************************************************************************" << endl;
        */
}

/*----------------------get method-------------------------------------*/

int parameters::get_Nxcell() const {
    return Nxcell;
}

int parameters::get_Nycell() const {
    return Nycell;
}

SCALAR parameters::get_dx() const {
    return dx;
}

SCALAR parameters::get_dy() const {
    return dy;
}

SCALAR parameters::get_T() const {
    return T;
}

SCALAR parameters::get_dtfix() const {
    return dt_fix;
}

SCALAR parameters::get_cflfix() const {
    return cfl_fix;
}

int parameters::get_nsave() const {
    return nsave;
}

int parameters::get_Lbound() const {
    return Lbound;
}
SCALAR parameters::get_left_imp_discharge() const {
    return left_imp_discharge/(Nycell*dy);
}
SCALAR parameters::get_left_imp_h() const {
    return left_imp_h;
}

int parameters::get_Rbound() const {
    return Rbound;
}
SCALAR parameters::get_right_imp_discharge() const {
    return right_imp_discharge/(Nycell*dy);
}
SCALAR parameters::get_right_imp_h() const {
    return right_imp_h;
}

int parameters::get_Bbound() const {
    return Bbound;
}
SCALAR parameters::get_bottom_imp_discharge() const {
    return bottom_imp_discharge/(Nxcell*dx);
}
SCALAR parameters::get_bottom_imp_h() const {
    return bottom_imp_h;
}

int parameters::get_Tbound() const {
    return Tbound;
}
SCALAR parameters::get_top_imp_discharge() const {
    return top_imp_discharge/(Nxcell*dx);
}
SCALAR parameters::get_top_imp_h() const {
    return top_imp_h;
}

int parameters::get_order() const {
    return order;
}

int parameters::get_flux() const {
    return flux;
}

int parameters::get_rec() const {
    return rec;
}
int parameters::get_lim() const {
    return lim;
}

SCALAR parameters::get_amortENO() const {
    return amortENO;
}

SCALAR parameters::get_modifENO() const {
    return modifENO;
}

int parameters::get_fric() const {
    return fric;
}
int parameters::get_fric_init() const {
    return fric_init;
}
SCALAR parameters::get_friccoef() const {
    return fric_coef;
}
string parameters::get_frictionNameFile(void) const {
    return fric_namefile;
}

string parameters::get_frictionNameFileS(void) const {
    return fric_namefile_short;
}

int parameters::get_inf() const {
    return inf;
}

int parameters::get_Ks_init() const {
    return Ks_init;
}

SCALAR parameters::get_Ks_coef() const {
    return Ks_coef;
}

string parameters::get_KsNameFile(void) const {
    return Ks_namefile;
}
string parameters::get_KsNameFileS() const {
    return Ks_namefile_short;
}

int parameters::get_dtheta_init() const {
    return dtheta_init;
}

SCALAR parameters::get_dtheta_coef() const {
    return dtheta_coef;
}
string parameters::get_dthetaNameFile(void) const {
    return dtheta_namefile;
}
string parameters::get_dthetaNameFileS() const {
    return dtheta_namefile_short;
}

int parameters::get_Psi_init() const {
    return Psi_init;
}

SCALAR parameters::get_Psi_coef() const {
    return Psi_coef;
}
string parameters::get_PsiNameFile(void) const {
    return Psi_namefile;
}
string parameters::get_PsiNameFileS() const {
    return Psi_namefile_short;
}

int parameters::get_retension_init() const {
    return retension_init;
}

SCALAR parameters::get_retension_coef() const {
    return retension_coef;
}
string parameters::get_retensionNameFile(void) const {
    return retension_namefile;
}
string parameters::get_retensionNameFileS() const {
    return retension_namefile_short;
}

int parameters::get_topo() const {
    return topo;
}
string parameters::get_topographyNameFile(void) const {
    return topography_namefile;
}
string parameters::get_topographyNameFileS(void) const {
    return topography_namefile_short;
}

int parameters::get_h() const {
    return h_initialization;
}
string parameters::get_hNameFile(void) const {
    return h_namefile;
}
string parameters::get_hNameFileS(void) const {
    return h_namefile_short;
}

int parameters::get_rain() const {
    return rain;
}
string parameters::get_rainNameFile(void) const {
    return rain_namefile;
}
string parameters::get_rainNameFileS(void) const {
    return rain_namefile_short;
}

string parameters::get_outputDirectory(void) const {
    return output_directory;
}
string parameters::get_suffix(void) const {
    return suffix_outputs;
}
int parameters::get_output() const {
    return output_format;
}

parameters::~parameters() {}
