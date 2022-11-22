#pragma once


#ifndef HZN_NODE_EDITOR_H
#define HZN_NODE_EDITOR_H


namespace Hzn
{
	class NodeEditor {
	private:

		
		GraphEditor::Options options;
		static GraphEditorDelegate delegate;
		static GraphEditor::ViewState viewState;
		static GraphEditor::FitOnScreen fit = GraphEditor::Fit_None;
		static bool showGraphEditor = true;
	
	
	public:
		static void showNodeEditor();
	};
}

#endif // !HZN_NODE_EDITOR_H