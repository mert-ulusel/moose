//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "SubdomainBoundingBoxGenerator.h"
#include "Conversion.h"
#include "CastUniquePointer.h"
#include "MooseUtils.h"

#include "libmesh/elem.h"

registerMooseObject("MooseApp", SubdomainBoundingBoxGenerator);

defineLegacyParams(SubdomainBoundingBoxGenerator);

InputParameters
SubdomainBoundingBoxGenerator::validParams()
{
  MooseEnum location("INSIDE OUTSIDE", "INSIDE");

  InputParameters params = MeshGenerator::validParams();

  params.addRequiredParam<MeshGeneratorName>("input", "The mesh we want to modify");
  params.addClassDescription("Changes the subdomain ID of elements either (XOR) inside or outside "
                             "the specified box to the specified ID.");
  params.addRequiredParam<RealVectorValue>(
      "bottom_left", "The bottom left point (in x,y,z with spaces in-between).");
  params.addRequiredParam<RealVectorValue>(
      "top_right", "The bottom left point (in x,y,z with spaces in-between).");
  params.addRequiredParam<subdomain_id_type>(
      "block_id", "Subdomain id to set for inside/outside the bounding box");
  params.addParam<SubdomainName>(
      "block_name", "Subdomain name to set for inside/outside the bounding box (optional)");
  params.addParam<MooseEnum>(
      "location", location, "Control of where the subdomain id is to be set");

  params.addParam<std::string>("integer_name",
                               "Element integer to be assigned (default to subdomain ID)");
  return params;
}

SubdomainBoundingBoxGenerator::SubdomainBoundingBoxGenerator(const InputParameters & parameters)
  : MeshGenerator(parameters),
    _input(getMesh("input")),
    _location(parameters.get<MooseEnum>("location")),
    _block_id(parameters.get<subdomain_id_type>("block_id")),
    _bounding_box(MooseUtils::buildBoundingBox(parameters.get<RealVectorValue>("bottom_left"),
                                               parameters.get<RealVectorValue>("top_right")))
{
}

std::unique_ptr<MeshBase>
SubdomainBoundingBoxGenerator::generate()
{
  std::unique_ptr<MeshBase> mesh = std::move(_input);

  if (isParamValid("integer_name"))
  {
    std::string integer_name = getParam<std::string>("integer_name");

    if (!mesh->has_elem_integer(integer_name))
      mooseError("Mesh does not have an element integer names as ", integer_name);

    unsigned int id = mesh->get_elem_integer_index(integer_name);

    // Loop over the elements
    for (const auto & elem : mesh->active_element_ptr_range())
    {
      bool contains = _bounding_box.contains_point(elem->centroid());
      if ((contains && _location == "INSIDE") || (!contains && _location == "OUTSIDE"))
        elem->set_extra_integer(id, _block_id);
    }
  }
  else
  {
    // Loop over the elements
    for (const auto & elem : mesh->active_element_ptr_range())
    {
      bool contains = _bounding_box.contains_point(elem->centroid());
      if ((contains && _location == "INSIDE") || (!contains && _location == "OUTSIDE"))
        elem->subdomain_id() = _block_id;
    }

    // Assign block name, if provided
    if (isParamValid("block_name"))
      mesh->subdomain_name(_block_id) = getParam<SubdomainName>("block_name");
  }

  return dynamic_pointer_cast<MeshBase>(mesh);
}
