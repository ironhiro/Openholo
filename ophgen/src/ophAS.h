#ifndef __ophAS_h
#define __ophAS_h


#include "ophGen.h"
#include "complex.h"
#include "sys.h"
#include "AngularC_types.h"



using namespace oph;



const complex<double> I = sqrt(complex<double>(-1, 0));

/**
* @addtogroup angularspectrum
//@{
* @details

* @section Introduction

The angular spectrum representation is a mathematical technique to describe optical elds
in homogeneous media.

![](@ref pics/ophgen/as/as1.jpg)

Result of the angular spectrum

![](@ref pics/ophgen/as/as_result.png)




*/
//! @} angular spectrum

/**
* @ingroup angularspectrum
* @brief Openholo Angular Spectrum based Compter-generated holography.
* @author
*/
class GEN_DLL ophAS : public ophGen 
{
private:
	coder::array<creal_T, 2U> im;
	unsigned char* res;
	double w;
	double h; 
	double wavelength;
	double knumber;
	double xi_interval;
	double eta_interval;
	double depth;
	double x;
	double y;
	double z;
	double amplitude;
	OphPointCloudConfig pc_config;
	OphPointCloudData pc_data;
	int n_points;
	bool is_CPU;
public:
	/**
	* @brief Constructor
	* @details Initialize variables.
	*/
	ophAS();
	/**
	* @brief Destructor
	*/
	virtual ~ophAS();
	/**
	* @brief read config files from xml
	* @param[in] fname file name of the xml files
	* @return Type: <B>bool</B>\n
	*				If the function succeeds, the return value is <B>true</B>\n
	*				If the function fails, the return value is <B>false</B>\n
	*/
	bool readConfig(const char* fname);
	/**
	* @brief override
	* @{
	* @brief Import Point Cloud Data Base File : *.ply file.
	* This Function is included memory location of Input Point Clouds.
	*/
	/**
	* @brief override
	* @param[in] _filename PointCloud(*.ply) input file path
	* @return number of Pointcloud (if it failed loading, it returned -1)
	*/
	int loadPoint(const char* fname);
	/**
	* @brief get the value of a variable is_CPU(true or false)
	* @details <pre>
	if is_CPU == true
	CPU implementation
	else
	GPU implementation </pre>
	*/
	void setmode(bool is_cpu);
	void ASCalculation(double w, double h, double wavelength, double knumber, double
		xi_interval, double eta_interval, double depth, coder::
		array<creal_T, 2U> &fringe, coder::array<creal_T, 2U>
		&b_AngularC);
	void RayleighSommerfield(double w, double h, double wavelength, double knumber, double
		xi_interval, double eta_interval, double depth, coder::
		array<creal_T, 2U> &fringe);
	void AngularSpectrum(double w, double h, double wavelength, double knumber, double
		xi_interval, double eta_interval, double depth, const coder::
		array<creal_T, 2U> &fringe, coder::array<creal_T, 2U>
		&b_AngularC);
	
	/**
	* @brief Generate a hologram, main funtion.
	*
	*/
	void generateHologram();
	void MemoryRelease();
	void ifft2(const coder::array<creal_T, 2U> &x, coder::array<creal_T, 2U> &y);
	void fft2_matlab(coder::array<creal_T, 2U> &x, coder::array<creal_T, 2U>
		&y);
	void eml_fftshift(coder::array<creal_T, 2U> &x, int dim);
	void ifftshift(coder::array<creal_T, 2U> &x);
	int save(const char * fname, uint8_t bitsperpixel, uchar* src, uint px, uint py);
	
	/**
	* @brief save to image
	* @param[in] fname file name of the image files to save
	*/
	void save(const char* fname);
};

#endif