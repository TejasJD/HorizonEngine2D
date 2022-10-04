#include "pch.h"
#include "LayerStack.h"

namespace Hzn
{
	LayerStack::LayerStack()
	{
	}
	
	//! destructor frees the memory allocated for every layer
	LayerStack::~LayerStack()
	{
		for (auto layer : m_Layers)
		{
			delete layer;
		}
	}
	
	void LayerStack::addLayer(Layer* layer)
	{
		m_Layers.emplace_front(layer);
		layer->onAttach();
	}

	void LayerStack::addOverlay(Layer* layer)
	{
		m_Layers.emplace_back(layer);
		layer->onAttach();
	}

	void LayerStack::removeLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
		}
	}

	void LayerStack::removeOverlay(Layer* layer)
	{
		removeLayer(layer);
	}
}