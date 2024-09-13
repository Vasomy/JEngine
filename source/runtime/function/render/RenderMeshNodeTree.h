#pragma once
#include"RenderCommon.h"

class RenderMeshNodeTree
{
public:
	Array<RenderMeshNode> Nodes;

	void Add(const RenderMeshNode& InNode);
};