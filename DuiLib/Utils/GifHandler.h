#ifndef __GIFHANDLER_H__
#define __GIFHANDLER_H__

#pragma once

namespace DuiLib 
{
	class UILIB_API CGifHandler
	{
	public:
		CGifHandler();

		~CGifHandler();

		int GetFrameCount();

		void AddFrameInfo(TImageInfo* pFrameInfo);

		TImageInfo* GetNextFrameInfo();

		TImageInfo* GetCurrentFrameInfo();

		TImageInfo* GetFrameInfoAt(int index);

	private:
		CStdPtrArray  ImageInfos;
		int nCurrentFrame;
		int nFrameCount;
		bool isDeleting;

	};
}

#endif