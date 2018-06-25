#include "StdAfx.h"

#include "GifHandler.h"

namespace DuiLib 
{
	CGifHandler::CGifHandler():
    nCurrentFrame(0),
	isDeleting(false)
	{
	}

	CGifHandler::~CGifHandler()
	{
		isDeleting = true;
		for(int i = 0; i < ImageInfos.GetSize(); i++)
		{
			CRenderEngine::FreeImage((TImageInfo *)ImageInfos.GetAt(i));
		}
	}

	int CGifHandler::GetFrameCount()
	{
		return ImageInfos.GetSize();
	}

	void CGifHandler::AddFrameInfo(TImageInfo* pFrameInfo)
	{
		if (pFrameInfo)
		{
			ImageInfos.Add(pFrameInfo);
		}	
	}

	TImageInfo* CGifHandler::GetNextFrameInfo()
	{
		if(isDeleting == false)
		{	
			int n = nCurrentFrame++;
			if (nCurrentFrame >= ImageInfos.GetSize())
			{
				nCurrentFrame = 0;
			}

			return (TImageInfo *)ImageInfos.GetAt(n);
		}

		return NULL;
	}

	TImageInfo* CGifHandler::GetCurrentFrameInfo()
	{
		if(isDeleting == false)
		{	
			return (TImageInfo *)ImageInfos.GetAt(nCurrentFrame);
		}

		return NULL;
	}

	TImageInfo* CGifHandler::GetFrameInfoAt(int index)
	{
		if(isDeleting == false)
		{	
			return (TImageInfo *)ImageInfos.GetAt(index);
		}

		return NULL;
	}
}