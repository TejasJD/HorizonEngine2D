#pragma once

#ifndef HZN_LAYER_STACK_H
#define HZN_LAYER_STACK_H

#include <list>

#include "HorizonEngine/Core/Layer.h"

namespace Hzn 
{
	/// <summary>
	/// Layer stack class
	/// </summary>
	class LayerStack
	{
	public:

		//! Layer iterator
		typedef std::list<Layer*>::iterator LayerIterator;
		//! A const layer iterator to ensure they don't change
		typedef std::list<Layer*>::const_iterator ConstLayerIterator;
		//! Reversed layor itetrator
		typedef std::list<Layer*>::reverse_iterator LayerReverseIterator;
		//! A reverse const layer iterator to ensure they don't change
		typedef std::list<Layer*>::const_reverse_iterator ConstLayerReverseIterator;
		
		/// <summary>
		/// constructor
		/// </summary>
		LayerStack();
		//!Destructor
		~LayerStack();

		
		//! Add a layer to the engine
		void addLayer(Layer* layer);
		//! Add an overlay to the engine
		void addOverlay(Layer* layer);
		//! Remove a layer to the engine
		void removeLayer(Layer* layer);
		//! Remove an overlay to the engine
		void removeOverlay(Layer* layer);

		//! Returns the beggining of the Layer stack
		LayerIterator begin() { return m_Layers.begin(); }
		//! Returns the end of the Layer stack
		LayerIterator end() { return m_Layers.end(); }

		//! Returns the reverve beggining of the Layer stack
		LayerReverseIterator rbegin() { return m_Layers.rbegin(); }
		//! Returns the reverse end of the Layer stack
		LayerReverseIterator rend() { return m_Layers.rend(); }
		
		//! Const layer iterators for const correctness
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
