#ifndef FLUIDPHYSICS_H
#define FLUIDPHYSICS_H

#include "IBK_LinearSpline.h"
#include "IBK_assert.h"
#include "IBK_math.h"

#include <array>

namespace IBK {

/*! Class for using spline values with two definition values (like a 3D space).*/
class MultiSpline {
public:
	/*! Create a non valid object. Cannot be used befor callinf set.*/
	MultiSpline() = default;

	/*! Set the internal database for the spline values.
		\param baseValues A vector of values for the first base (x1).
		\param values Vector of pairs of vectors for the values. The size of the outer vector must be the same as the size of the first base vector (\a basevalues).
		The first part of the pair is the second base (x2) and the second part of the pair the values to be used (y).
		It represents a function like this:   y = f(x1,x2).
	*/
	void set(const std::vector<double> baseValues, const std::vector<std::pair<std::vector<double>,std::vector<double>>>& values)
	{
		IBK_ASSERT(baseValues.size() == values.size());
		IBK_ASSERT(!baseValues.empty());

		m_baseValues = baseValues;
		for(const auto& lp : values) {
			m_values.push_back(IBK::LinearSpline());
			m_values.back().setValues(lp.first,lp.second);
		}
	}

	/*! It returns the value for the given first and second base values (x1 and x2).
	 *  \param x1 First base (see baseValues in set()).
	 *  \param x2 Second base (see first part of the pair of values in set()).
	*/
	double value(double x1, double x2) {
		std::pair<size_t,size_t> indices = indexArea(x1);
		if(indices.first == indices.second) {
			IBK_ASSERT(indices.first < m_values.size());
			return m_values[indices.first].value(x2);
		}
		double fact = (m_values[indices.second].value(x2) - m_values[indices.first].value(x2)) /
				(m_baseValues[indices.second] - m_baseValues[indices.first]);
		double res = m_values[indices.second].value(x2) - (m_baseValues[indices.second] - x1) * fact;
		return res;
	}

private:

	std::vector<double>				m_baseValues;	///< Vector for first base (x1)
	std::vector<IBK::LinearSpline>	m_values;		///< Vector of spline with second base (x2) and values (y)

	/*! Find out the place of the given x1 value in the first base vector.
	 *  The result show left and right bound of the indices in the vector. The indices can be equel in case the value lays exactly at a vector value.
		Internal function which is used from value().
	*/
	std::pair<size_t,size_t> indexArea(double x1) {
		if(m_baseValues.empty() || x1 < m_baseValues.front() || IBK::near_zero(x1))
			return {0,0};
		if(x1 > m_baseValues.back() || IBK::near_equal(m_baseValues.back(),x1))
			return {m_baseValues.size()-1,m_baseValues.size()-1};
		for(size_t i=0; i<m_baseValues.size(); ++i) {
			if(IBK::near_equal(m_baseValues[i],x1))
				return {i,i};
		}
		size_t index = std::lower_bound(m_baseValues.begin(), m_baseValues.end(), x1)-m_baseValues.begin();
		return {index,index+1};
	}
};

/*!	\file IBK_FluidPhysics.h
	\brief Physical constants and functions needed to calculate heat transport and wall friction of fluidy in pipes.
*/

/*! Calculates Reynolds number [-] of a moving fluid.
	\param v mean fluid flow velocity [m/s]
	\param kinVis fluid kinematic viscosity [m2/s]
	\param l characteristic length [m]
*/
double ReynoldsNumber(const double &v, const double &kinVis, const double &l);

/*! Calculates Prandtl number [-] of a moving fluid.
	\param kinVis fluid kinematic viscosity [m2/s]
	\param cp fluid specific heat capacity [J/kgK]
	\param lambda fluid thermal conductivity [W/mK]
	\param rho fluid mass density [kg/m3]
*/
double PrandtlNumber(const double &kinVis, const double &cp, const double &lambda, const double &rho);

/*! Calculates nusselt number [-] for laminar fluid flow through a pipe.
	Nusselt number has fixed minimum to laminar
	\param reynolds Reynolds number [-]
	\param prandtl Prandtl number [-]
	\param l Characteristic length [m]
	\param d Pipe outside diameter [m]
*/
double NusseltNumberLaminar(const double &reynolds, const double &prandtl, const double &l, const double &d);

/*! Calculates nusselt number [-] for a turbulent fluid through a pipe. Reynolds should not be 0! This function is only
 * called by NusseltNumber() in case Re > RE_LAMINAR.
	Nusselt number has fixed minimum to laminar
	\param reynolds Reynolds number [-]
	\param prandtl Prandtl number [-]
	\param l Characteristic length [m]
	\param d Pipe outside diameter [m]
*/
double NusseltNumberTurbulent(const double &reynolds, const double &prandtl, const double &l, const double &d);

/*! Calculates nusselt number [-] for a turbulent fluid through a pipe.
	Nusselt number has fixed minimum to laminar
	\param reynolds Reynolds number [-]
	\param prandtl Prandtl number [-]
	\param l Characteristic length [m]
	\param d Pipe outside diameter [m]
*/
double NusseltNumber(const double &reynolds, const double &prandtl, const double &l, const double &d);

/*! Calculates the darcy friction factor [-] according to swamee-jain equation (approximation of colebrook-white)
	\param reynolds Reynolds number [-]
	\param d Pipe outside diameter [m]
	\param roughness Pipe wall roughness [m]
*/
double FrictionFactorSwamee(const double &reynolds, const double &d, const double &roughness);

/*! Class which can deliver fluid properties for a Water-Glycol mixture depending on temperature.*/
class WaterMixProperties {
public:
	/*! Which kind of Glycol is used.*/
	enum GlycolKind {
		GK_Ethylene,
		GK_Propylene,
		GK_Count
	};

	/*! Constructor with fixed values for Glycol kind and amount.
	 *  \param glycolContent Glycol content in volume fraction (0.2 means 20Vol%).
	 *  \param glycolKind Kind of the used glycol
	*/
	WaterMixProperties(double glycolContent = 0, GlycolKind glycolKind = GK_Ethylene);

	/*! Density of the mixture in kg/m3 depending on given temperature.
		\param temperature Temperature in °C.
	*/
	double density(double temperature);

	/*! Dynamic viscosity of the mixture in Pa*s depending on given temperature.
		\param temperature Temperature in °C.
	*/
	double dynViscosity(double temperature);

	/*! Kinematic viscosity of the mixture in m2/s depending on given temperature.
		\param temperature Temperature in °C.
	*/
	double kinViscosity(double temperature);

	/*! Thermal conductivity of the mixture in W/mK depending on given temperature.
		\param temperature Temperature in °C.
	*/
	double lambda(double temperature);

	/*! Prandtl number of the mixture depending on given temperature.
		\param temperature Temperature in °C.
		Is calculated from dynVis*cp/lambda
	*/
	double prandtl(double temperature);

	/*! Specific heat capacity of the mixture in J/kgK depending on given temperature.
		\param temperature Temperature in °C.
	*/
	double heatCapacity(double temperature);

	/*! Freezing temperature in °C for the given mixture.*/
	double freezeTemperature();

private:
	double								m_glycolContent;		///< Glycol content as volume fraction
	GlycolKind							m_glycolKind;			///< Glycol kind (Ethylene or Propylene)
	std::array<LinearSpline, GK_Count>	m_freezeTemperature;	///< Vector of freezing temperature
	std::array<MultiSpline, GK_Count>	m_densities;			///< Densities in kg/m3
	std::array<MultiSpline, GK_Count>	m_dynViscosity;			///< Dynamic viscosity in Pa*s
	std::array<MultiSpline, GK_Count>	m_kinViscosity;			///< Kinematic viscosity in m2/s
	std::array<MultiSpline, GK_Count>	m_lambda;				///< Thermal conductivity in W/mK
	std::array<MultiSpline, GK_Count>	m_cp;					///< Specific heat capacity in J/kgK
};


} // namespace IBK

#endif // FLUIDPHYSICS_H
