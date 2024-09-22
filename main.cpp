#include<runtime/core/JCores.h>
#include<iostream>
#include<runtime/function/engine/Engine.h>
#include<comdef.h>
#include<d3d12.h>
#include<editor/Editor.h>

#define LogErrorWhenThrow
int main()
{
	try {
		
		Editor editor;
		editor.Init();
		Engine engine(&editor);
		engine.Init();
		engine.Run();
	}
    catch (DxException& e)
    {
		
        MessageBox(nullptr, e.ToString().c_str(), L"HR FAILED", MB_OK);

        return 0;
    }
}