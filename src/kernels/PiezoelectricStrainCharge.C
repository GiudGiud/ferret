/*
   This file is part of FERRET, an add-on module for MOOSE

   FERRET is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

   For help with FERRET please contact J. Mangeri <john.m.mangeri@gmail.com>
   and be sure to track new changes at github.com/mangerij/ferret

**/

#include "PiezoelectricStrainCharge.h"
#include "ComputePiezostrictiveTensor.h"

class PiezoelectricStrainCharge;

registerMooseObject("FerretApp", PiezoelectricStrainCharge);

InputParameters PiezoelectricStrainCharge::validParams()
{
  InputParameters params = Kernel::validParams();
  params.addClassDescription("Calculates a residual contribution due to a charge arising via piezoelectric coupling in the Poisson equation.");
  params.addRequiredCoupledVar("disp_x", "The x component of the displacement");
  params.addRequiredCoupledVar("disp_y", "The y component of the displacement");
  params.addCoupledVar("disp_z", 0.0, "The z component of the displacement");
  params.addParam<Real>("len_scale", 1.0, "the length scale of the unit");
  return params;
}

PiezoelectricStrainCharge::PiezoelectricStrainCharge(const InputParameters & parameters)
  :Kernel(parameters),
   _piezostrictive_tensor(getMaterialProperty<RankThreeTensor>("piezostrictive_tensor")),
   _disp_x_var(coupled("disp_x")),
   _disp_y_var(coupled("disp_y")),
   _disp_z_var(coupled("disp_z")),
   _disp_x_grad(coupledGradient("disp_x")),
   _disp_y_grad(coupledGradient("disp_y")),
   _disp_z_grad(coupledGradient("disp_z")),
   _len_scale(getParam<Real>("len_scale"))
{
}

Real
PiezoelectricStrainCharge::computeQpResidual()
{
  Real sum = 0.0;
  for (unsigned int j = 0; j < 3; ++j)
  {
    sum += _grad_test[_i][_qp](j)* std::pow(2,-1.0) *((2 * _piezostrictive_tensor[_qp](j,0,0) * _disp_x_grad[_qp](0) + _piezostrictive_tensor[_qp](j,0,1) * (_disp_x_grad[_qp](1) + _disp_y_grad[_qp](0)) + _piezostrictive_tensor[_qp](j,0,2) * (_disp_x_grad[_qp](2) + _disp_z_grad[_qp](0))) + (_piezostrictive_tensor[_qp](j,1,0) * (_disp_y_grad[_qp](0) + _disp_x_grad[_qp](1)) + (2 * _piezostrictive_tensor[_qp](j,1,1) * _disp_y_grad[_qp](1)) + _piezostrictive_tensor[_qp](j,1,2) * (_disp_y_grad[_qp](2)+ _disp_z_grad[_qp](1))) + (_piezostrictive_tensor[_qp](j,2,0) * (_disp_z_grad[_qp](0) + _disp_x_grad[_qp](2)) + (_piezostrictive_tensor[_qp](j,2,1) * (_disp_z_grad[_qp](1) + _disp_y_grad[_qp](2))) + (2 * _piezostrictive_tensor[_qp](j,2,2) * _disp_z_grad[_qp](2))));
  }
  return sum;
}

Real
PiezoelectricStrainCharge::computeQpJacobian()
{
  return 0.0;
}



Real
PiezoelectricStrainCharge::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _disp_x_var)
    return std::pow(2,-1.0)*(_grad_test[_i][_qp](0) * (2 * _piezostrictive_tensor[_qp](0,0,0) * _grad_phi[_j][_qp](0) + _piezostrictive_tensor[_qp](0,0,1) * _grad_phi[_j][_qp](1) + _piezostrictive_tensor[_qp](0,0,2) * _grad_phi[_j][_qp](2) + _piezostrictive_tensor[_qp](0,1,0) * _grad_phi[_j][_qp](1) + _piezostrictive_tensor[_qp](0,2,0) * _grad_phi[_j][_qp](2)) + _grad_test[_i][_qp](1) * (2 * _piezostrictive_tensor[_qp](1,0,0) * _grad_phi[_j][_qp](0) + _piezostrictive_tensor[_qp](1,0,1) * _grad_phi[_j][_qp](1) + _piezostrictive_tensor[_qp](1,0,2) * _grad_phi[_j][_qp](2) + _piezostrictive_tensor[_qp](1,1,0) * _grad_phi[_j][_qp](1) + _piezostrictive_tensor[_qp](1,2,0) * _grad_phi[_j][_qp](2)) + _grad_test[_i][_qp](2) * (2 * _piezostrictive_tensor[_qp](2,0,0) * _grad_phi[_j][_qp](0) + _piezostrictive_tensor[_qp](2,0,1) * _grad_phi[_j][_qp](1) + _piezostrictive_tensor[_qp](2,0,2) * _grad_phi[_j][_qp](2) + _piezostrictive_tensor[_qp](2,1,0) * _grad_phi[_j][_qp](1) + _piezostrictive_tensor[_qp](2,2,0) * _grad_phi[_j][_qp](2)));
  else if (jvar==_disp_y_var)
  {
    return std::pow(2,-1.0) * (_grad_test[_i][_qp](0) * (_piezostrictive_tensor[_qp](0,0,1) * _grad_phi[_j][_qp](0) + _piezostrictive_tensor[_qp](0,1,0) * _grad_phi[_j][_qp](0) + 2 * _piezostrictive_tensor[_qp](0,1,1) * _grad_phi[_j][_qp](1) + _piezostrictive_tensor[_qp](0,1,2) * _grad_phi[_j][_qp](2) + _piezostrictive_tensor[_qp](0,2,1) * _grad_phi[_j][_qp](2)) + _grad_test[_i][_qp](1) * (_piezostrictive_tensor[_qp](1,0,1) * _grad_phi[_j][_qp](0) + _piezostrictive_tensor[_qp](1,1,0) * _grad_phi[_j][_qp](0) + 2 * _piezostrictive_tensor[_qp](1,1,1) * _grad_phi[_j][_qp](1) + _piezostrictive_tensor[_qp](1,1,2) * _grad_phi[_j][_qp](2) + _piezostrictive_tensor[_qp](1,2,1) * _grad_phi[_j][_qp](2)) + _grad_test[_i][_qp](2) * (_piezostrictive_tensor[_qp](2,0,1) * _grad_phi[_j][_qp](0) + _piezostrictive_tensor[_qp](2,1,0) * _grad_phi[_j][_qp](0) + 2 * _piezostrictive_tensor[_qp](2,1,1) * _grad_phi[_j][_qp](1) + _piezostrictive_tensor[_qp](2,1,2) * _grad_phi[_j][_qp](2) + _piezostrictive_tensor[_qp](2,2,1) * _grad_phi[_j][_qp](2)));
  }
  else if (jvar==_disp_z_var)
  {
    return std::pow(2,-1.0) * (_grad_test[_i][_qp](0) * (_piezostrictive_tensor[_qp](0,0,2) * _grad_phi[_j][_qp](0) + _piezostrictive_tensor[_qp](0,1,2) * _grad_phi[_j][_qp](1) + _piezostrictive_tensor[_qp](0,2,0) * _grad_phi[_j][_qp](0) + _piezostrictive_tensor[_qp](0,2,1) * _grad_phi[_j][_qp](1) + 2 * _piezostrictive_tensor[_qp](0,2,2) * _grad_phi[_j][_qp](1)) + _grad_test[_i][_qp](1) * (_piezostrictive_tensor[_qp](1,0,2) * _grad_phi[_j][_qp](0) + _piezostrictive_tensor[_qp](1,1,2) * _grad_phi[_j][_qp](1) + _piezostrictive_tensor[_qp](1,2,0) * _grad_phi[_j][_qp](0) + _piezostrictive_tensor[_qp](1,2,1) * _grad_phi[_j][_qp](1) + 2 * _piezostrictive_tensor[_qp](1,2,2) * _grad_phi[_j][_qp](1)) + _grad_test[_i][_qp](2) * (_piezostrictive_tensor[_qp](2,0,2) * _grad_phi[_j][_qp](0) + _piezostrictive_tensor[_qp](2,1,2) * _grad_phi[_j][_qp](1) + _piezostrictive_tensor[_qp](2,2,0) * _grad_phi[_j][_qp](0) + _piezostrictive_tensor[_qp](2,2,1) * _grad_phi[_j][_qp](1) + 2 * _piezostrictive_tensor[_qp](2,2,2) * _grad_phi[_j][_qp](2)));
  }
  else
    return 0.0;
}
