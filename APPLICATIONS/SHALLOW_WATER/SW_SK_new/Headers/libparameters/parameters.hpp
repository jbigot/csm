#ifndef Class_parameters
#define Class_parameters

#include "misc.hpp"

class parameters {
protected :
    //-------------------
    static parameters * singleton;
    parameters();
    //-------------------
    SCALAR cfl_fix,dt_fix;
    int Nxcell, Nycell, nsave;
    SCALAR T, dx, dy;

    int Lbound, Rbound, Bbound, Tbound;
    SCALAR left_imp_discharge, left_imp_h;
    SCALAR right_imp_discharge ,right_imp_h;
    SCALAR bottom_imp_discharge ,bottom_imp_h;
    SCALAR top_imp_discharge ,top_imp_h;

    int flux,order,rec,lim;
    SCALAR amortENO, modifENO;

    int fric,fric_init;
    SCALAR fric_coef;
    string fric_namefile, fric_namefile_short;

    int  inf;
    string Ks_namefile, Ks_namefile_short;
    int Ks_init;
    SCALAR Ks_coef;
    string dtheta_namefile, dtheta_namefile_short;
    int dtheta_init;
    SCALAR dtheta_coef;
    string Psi_namefile, Psi_namefile_short;
    int Psi_init;
    SCALAR Psi_coef;

    string retension_namefile, retension_namefile_short;
    int retension_init;
    SCALAR retension_coef;

    int topo;
    string topography_namefile, topography_namefile_short;

    int h_initialization;
    string h_namefile, h_namefile_short;

    int rain;
    string rain_namefile, rain_namefile_short;

    string output_directory, suffix_outputs;
    char * tempo_topo, * tempo_huv, * tempo_rain, * tempo_suffix, * suffix;

    string  namefile;
    int output_format;


public :
    static parameters * getSingleton();
    void setparameters(const char *);
    virtual ~parameters();
    int get_Nxcell() const ;
    int get_Nycell() const ;
    int get_nsave() const ;
    SCALAR get_T() const ;
    SCALAR get_dtfix() const ;
    SCALAR get_cflfix() const ;
    SCALAR get_dx() const ;
    SCALAR get_dy() const ;

    int get_Lbound() const ;
    int get_Rbound() const ;
    int get_Bbound() const ;
    int get_Tbound() const ;
    SCALAR get_left_imp_discharge() const ;
    SCALAR get_left_imp_h() const ;
    SCALAR get_right_imp_discharge() const ;
    SCALAR get_right_imp_h() const ;
    SCALAR get_bottom_imp_discharge() const ;
    SCALAR get_bottom_imp_h() const ;
    SCALAR get_top_imp_discharge() const ;
    SCALAR get_top_imp_h() const ;

    int get_flux() const ;
    int get_order() const ;
    int get_rec() const ;

    int get_lim() const ;
    SCALAR get_amortENO() const ;
    SCALAR get_modifENO() const;

    int get_fric() const ;
    int get_fric_init() const ;
    SCALAR get_friccoef() const ;
    string get_frictionNameFile(void) const;
    string get_frictionNameFileS(void) const;

    int get_inf() const ;
    int get_Ks_init() const ;
    SCALAR get_Ks_coef() const ;
    string get_KsNameFile(void) const;
    string get_KsNameFileS() const;
    int get_dtheta_init() const ;
    SCALAR get_dtheta_coef() const ;
    string get_dthetaNameFile(void) const;
    string get_dthetaNameFileS() const;
    int get_Psi_init() const ;
    SCALAR get_Psi_coef() const ;
    string get_PsiNameFile(void) const;
    string get_PsiNameFileS() const;

    int get_retension_init() const ;
    SCALAR get_retension_coef() const ;
    string get_retensionNameFile(void) const;
    string get_retensionNameFileS() const;

    int get_topo() const ;
    int get_h() const ;
    int get_rain() const ;

    string get_topographyNameFile(void) const;
    string get_topographyNameFileS() const;
    string get_hNameFile(void) const;
    string get_hNameFileS(void) const;
    string get_rainNameFile(void) const;
    string get_rainNameFileS(void) const;

    string get_outputDirectory(void) const;
    string get_suffix(void) const;
    int get_output() const ;
};
#endif
