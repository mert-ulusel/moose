//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "ODEKernel.h"

class ImplicitODEy : public ODEKernel
{
public:
  static InputParameters validParams();

  ImplicitODEy(const InputParameters & parameters);
  virtual ~ImplicitODEy();

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  unsigned int _x_var;
  VariableValue & _x;
};
