#include "pch.h"
#include "LayerStack.h"

namespace Hzn
{
	LayerStack::LayerStack()
	{
		m_TopLayer = m_Layers.begin();
	}
	
	//! destructor frees the memory allocated for every layer
	LayerStack::~LayerStack()
	{
		for (auto layer : m_Layers)
		{
			delete layer;
		}
	}
	
	//! push layer into the layer stack.
	void LayerStack::addLayer(Layer* layer)
	{
		m_TopLayer = m_Layers.emplace(m_TopLayer, layer);
		++m_TopLayer;
	}

	//! push overlay into the layer stack.
	void LayerStack::addOverlay(Layer* layer)
	{
		m_Layers.emplace_back(layer);
	}

	void LayerStack::removeLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			if (it == m_TopLayer)
			{
				m_TopLayer = m_Layers.erase(m_TopLayer);
				--m_TopLayer;
			}
			else
			{
				m_Layers.erase(it);
			}
		}
	}

	void LayerStack::removeOverlay(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
		}
	}
}