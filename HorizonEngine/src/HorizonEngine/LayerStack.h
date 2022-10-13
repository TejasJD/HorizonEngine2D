#pragma once

#ifndef HZN_LAYER_STACK_H
#define HZN_LAYER_STACK_H

#include "HorizonEngine/Layer.h"

namespace Hzn 
{
	class LayerStack
	{
	public:
		typedef std::list<Layer*>::iterator LayerIterator;
		typedef std::list<Layer*>::const_iterator ConstLayerIterator;
		typedef std::list<Layer*>::reverse_iterator LayerReverseIterator;
		typedef std::list<Layer*>::const_reverse_iterator ConstLayerReverseIterator;

		LayerStack();
		~LayerStack();

		void addLayer(Layer* layer);
		void addOverlay(Layer* layer);
		void removeLayer(Layer* layer);
		void removeOverlay(Layer* layer);

		LayerIterator begin() { return m_Layers.begin(); }
		LayerIterator end() { return m_Layers.end(); }

		LayerReverseIterator rbegin() { return m_Layers.rbegin(); }
		LayerReverseIterator rend() { return m_Layers.rend(); }
		
		ConstLayerIterator begin() const { return m_Layers.begin(); }
		ConstLayerIterator end() const { return m_Layers.begin(); }

		ConstLayerIterator cbegin() const { return m_Layers.begin(); }
		ConstLayerIterator cend() const { return m_Layers.begin(); }

		ConstLayerReverseIterator rbegin() const { return m_Layers.rbegin(); }
		ConstLayerReverseIterator rend() const { return m_Layers.rend(); }

		ConstLayerReverseIterator crbegin() const { return m_Layers.rbegin(); }
		ConstLayerReverseIterator crend() const { return m_Layers.rend(); }

	private:
		std::list<Layer*> m_Layers;
		LayerIterator m_TopLayer;
	};
}
#endif // !HZN_LAYER_STACK_H
