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

   For help with FERRET please contact J. Mangeri <johnma@dtu.dk>
   and be sure to track new changes at github.com/mangerij/ferret

**/

#include "Birefringence.h"

registerMooseObject("FerretApp", Birefringence);

InputParameters Birefringence::validParams()

{
  InputParameters params = AuxKernel::validParams();
  params.addClassDescription("Computes the difference between refractive indices (birefringence).");
  params.addRequiredCoupledVar("per1", "first perpendicular direction to propagation");
  params.addRequiredCoupledVar("per2", "second perpendicular direction to propagation");
  return params;
}

Birefringence::Birefringence(const InputParameters & parameters) :
  AuxKernel(parameters),
  _var1(coupledValue("per1")),
  _var2(coupledValue("per2"))
{
}

Real
Birefringence::computeValue()
{
  return _var2[_qp] - _var1[_qp];
}


