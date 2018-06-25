#include "stdafx.h"
#include "Room.h"

InstanceRealize(CRoomInfo);

CRoomInfo::CRoomInfo()
{

}

void CRoomInfo::init()
{
	SetInfo("10101", 
		L"enter_������~~north_�����~~~"
		L"~��~|~~~"
		L"west_�����~--~_����㳡~--~east_�����~"
		L"~~|~~~"
		L"~~south_�ϴ��~~~");
	SetInfo("10102", 		
		L"~~north_�����~~~"
		L"~~|~~~"
		L"west_ʢ������~--~_�����~--~east_�͵�~"
		L"~~|~~~"
		L"~~south_����㳡~~~");
	SetInfo("10103", 		
		L"~~north_������~~~"
		L"~~|~~~"
		L"west_�������~--~_�����~--~east_����¥~"
		L"~~|~~~"
		L"~~south_�����~~~");
	SetInfo("10104", 		
		L"~~north_(����)~~~"
		L"~~|~~~"
		L"~~_������~~~"
		L"~~|~~~"
		L"~~south_�����~~~");
	SetInfo("10105", 		
		L"~~north_����㳡~~~"
		L"~~|~~~"
		L"west_Ǯׯ~--~_�ϴ��~--~east_����~"
		L"~~|~~~"
		L"~~south_�ϴ��~~~");
	SetInfo("10106", 		
		L"~~north_�ϴ��~~~"
		L"~~|~~~"
		L"west_������~--~_�ϴ��~--~east_�ٱ�ի~"
		L"~~|~~~"
		L"~~south_�ϳ���~~~");
	SetInfo("10107", 		
		L"~~north_�ϴ��~~~"
		L"~~|~~~"
		L"~~_�ϳ���~~~"
		L"~~~~~"
		L"~~~~~");
	SetInfo("10108", 		
		L"~~~~~"
		L"~~~~~"
		L"west_�����~--~_�����~--~east_����㳡~"
		L"~~~~~"
		L"~~~~~");
	SetInfo("10109", 	
		L"~~north_��Ҵ���~~~"
		L"~~|~~~"
		L"west_������~--~_�����~--~east_�����~"
		L"~~|~~~"
		L"~~south_��Ӫ����~~~");
	SetInfo("10110", 	
		L"~~~~~"
		L"~~~~~"
		L"~~_������~--~east_�����~"
		L"~~~~~"
		L"~~~~~");
	SetInfo("10111", 	
		L"~~north_ҽ��~~~"
		L"~~|~~~"
		L"west_����㳡~--~_�����~--~east_�����~"
		L"~~|~~~"
		L"~~south_�ӻ���~~~");
	SetInfo("10112", 	
		L"~~north_��¹��Ժ~~~"
		L"~~|~~~"
		L"west_�����~--~_�����~--~east_������~"
		L"~~|~~~"
		L"~~south_��ݴ���~~~");
	SetInfo("10113", 	
		L"~~~~~"
		L"~~~~~"
		L"west_�����~--~_������~~~"
		L"~~~~~"
		L"~~~~~");
	SetInfo("10114", 	
		L"~~up_�ͷ�~~~"
		L"~~=~~~"
		L"west_�����~--~_�͵�~~~"
		L"~~~~~"
		L"~~~~~");
	SetInfo("10115", 	
		L"~~up_�Ƶ��¥~~~"
		L"~~=~~~"
		L"west_�����~--~_����¥~~~"
		L"~~~~~"
		L"~~~~~");
	SetInfo("10116", 	
		L"~~~~~"
		L"~~~~~"
		L"~~_�Ƶ��¥~~~"
		L"~~=~~~"
		L"~~down_����¥~~~");
	SetInfo("10117", 	
		L"~~~~~"
		L"~~~~~"
		L"~~_�������~--~east_�����~"
		L"~~~~~"
		L"~~~~~");
	SetInfo("10118", 	
		L"~~~~~"
		L"~~~~~"
		L"~~_������~~~"
		L"~~~��~~"
		L"~~~~out_����㳡~");
	SetInfo("10119", 		
		L"~~enter_�︮����~~~"
		L"~~��~~~"
		L"~~_��Ҵ���~~~"
		L"~~|~~~"
		L"~~south_�����~~~");
	SetInfo("10120", 		
		L"~~north_��լ~~~"
		L"~~|~~~"
		L"west_��Ժ~--~_�︮����~--~east_����~"
		L"~~��~~~"
		L"~~out_��Ҵ���~~~");
	SetInfo("10121", 	
		L"~~~~~"
		L"~~~~~"
		L"~~_��Ժ~--~east_�︮����~"
		L"~~~~~"
		L"~~~~~");
	SetInfo("10122", 	
		L"~~~~~"
		L"~~~~~"
		L"west_�︮����~--~_����~~~"
		L"~~~~~"
		L"~~~~~");
	SetInfo("10123", 	
		L"~~~~~"
		L"~~~~~"
		L"~~_��լ~~~"
		L"~~|~~~"
		L"~~south_�︮����~~~");
	SetInfo("10124", 		
		L"~~north_�����~~~"
		L"~~|~~~"
		L"~~_��Ӫ����~~~"
		L"~~|~~~"
		L"~~south_��Ӫ~~~");
	SetInfo("10125", 		
		L"~~north_��Ӫ����~~~"
		L"~~|~~~"
		L"~~_��Ӫ~~~"
		L"~~|~~~"
		L"~~south_������~~~");
	SetInfo("10126", 		
		L"~~north_��Ӫ~~~"
		L"~~|~~~"
		L"~~_������~~~"
		L"~~~~~"
		L"~~~~~");
	SetInfo("10127", 	
		L"~~~~~"
		L"~~~~~"
		L"~~_Ǯׯ~--~east_�ϴ��~"
		L"~~~~~"
		L"~~~~~");
	SetInfo("10128", 	
		L"~~~~~"
		L"~~~~~"
		L"west_�ϴ��~--~_����~~~"
		L"~~~~~"
		L"~~~~~");
	SetInfo("10129", 	
		L"~~~~~"
		L"~~~~~"
		L"~~_ʢ������~--~east_�����~"
		L"~~~~~"
		L"~~~~~");
	SetInfo("10130", 	
		L"~~~~~"
		L"~~~~~"
		L"west_�ϴ��~--~_�ٱ�ի~~~"
		L"~~~~~"
		L"~~~~~");
	SetInfo("10131", 	
		L"~~~~~"
		L"~~~~~"
		L"~~_������~--~east_�ϴ��~"
		L"~~~~~"
		L"~~~~~");
	SetInfo("10132", 	
		L"~~~~~"
		L"~~~~~"
		L"~~_ҽ��~~~"
		L"~~|~~~"
		L"~~south_�����~~~");
	SetInfo("10133", 		
		L"~~north_�����~~~"
		L"~~|~~~"
		L"~~_�ӻ���~~~"
		L"~~~~~"
		L"~~~~~");
	SetInfo("10134", 	
		L"~~~~~"
		L"~~~~~"
		L"~~_��¹��Ժ~~~"
		L"~~|~~~"
		L"~~south_�����~~~");
	SetInfo("10136", 		
		L"~~north_�����~~~"
		L"~~|~~~"
		L"~~_��ݴ���~~~"
		L"~~|~~~"
		L"~~south_���䳡~~~");
	SetInfo("10137", 		
		L"~~north_��ݴ���~~~"
		L"~~|~~~"
		L"~~_���䳡~~~"
		L"~~|~~~"
		L"~~south_�������~~~");
	SetInfo("10138", 		
		L"~~north_���䳡~~~"
		L"~~|~~~"
		L"~~_�������~~~"
		L"~~~~~"
		L"~~~~~");
	SetInfo("10139", 	
		L"~~~~~"
		L"~~~~~"
		L"~~_�ͷ�~~~"
		L"~~=~~~"
		L"~~down_�͵�~~~");

	SetInfo("10201", 
		L"~~north_��ʯ���~~~"
		L"~~|~~~"
		L"~~_��ʯ���~~~"
		L"~~|~~~"
		L"~~south_(����)~~~");
	SetInfo("10202", 
		L"~~north_�ϳ���~~~"
		L"~~|~~~"
		L"~~_��ʯ���~~~"
		L"~~|~~~"
		L"~~south_��ʯ���~~~");
	SetInfo("10203", 
		L"~~north_�ϴ��~~~"
		L"~~|~~~"
		L"~~_�ϳ���~~~"
		L"~~|~~~"
		L"~~south_��ʯ���~~~");
	SetInfo("10204", 
		L"~~north_��¥���~~~"
		L"~~|~~~"
		L"~~_�ϴ��~--~east_������~"
		L"~~|~~~"
		L"~~south_�ϳ���~~~");
	SetInfo("10205", 
		L"~~north_��¥~~~"
		L"~~|~~~"
		L"west_�����~--~_��¥���~--~east_�����~"
		L"~~|~~~"
		L"~~south_�ϴ��~~~");
	SetInfo("10206", 
		L"~~north_��¥���~~~"
		L"~~|~~~"
		L"west_����~--~_��¥~--~east_��¥���~"
		L"~~|~~~"
		L"~~south_��¥���~~~");
	SetInfo("10207", 
		L"~~north_������~~~"
		L"~~|~~~"
		L"west_�׸�~--~_��¥���~~~"
		L"~~|~~~"
		L"~~south_��¥~~~");
	SetInfo("10208", 
		L"~~~~~"
		L"~~~~~"
		L"~~_������~~~"
		L"~~|~~~"
		L"~~south_��¥���~~~");
	SetInfo("10209", 
		L"~~~~~"
		L"~~~~~"
		L"west_�����~--~_�����~--~east_��¥���~"
		L"~~~~~"
		L"~~~~~");
	SetInfo("10210", 
		L"~~~~~"
		L"~~~~~"
		L"~~_����~--~east_��¥~"
		L"~~~~~"
		L"~~~~~");
	SetInfo("10211", 
		L"~~~~~"
		L"~~~~~"
		L"~~_�׸�~--~east_��¥���~"
		L"~~~~~"
		L"~~~~~");
	SetInfo("10212", 
		L"~~~~~"
		L"~~~~~"
		L"west_������~--~_�����~--~east_�����~"
		L"~~~~~"
		L"~~~~~");
	SetInfo("10213", 
		L"~~~~~"
		L"~~~~~"
		L"~~_������~--~east_�����~"
		L"~~~~~"
		L"~~~~~");
	SetInfo("10214", 
		L"~~~~~"
		L"~~~~~"
		L"west_�ϴ��~--~_������~~~"
		L"~~~~~"
		L"~~~~~");
	SetInfo("10215", 
		L"~~north_��¥���~~~"
		L"~~|~~~"
		L"west_��¥���~--~_�����~--~east_������~"
		L"~~~~~"
		L"~~~~~");
	SetInfo("10216", 
		L"~~enter_�ջ���~~~"
		L"~~��~~~"
		L"west_��¥~--~_��¥���~~~"
		L"~~|~~~"
		L"~~south_�����~~~");
	SetInfo("10217", 
		L"~~~~~"
		L"~~~~~"
		L"~~_�ջ���~~~"
		L"~~��~~~"
		L"~~out_��¥���~~~");
	SetInfo("10218", 
		L"~~~~~"
		L"~~~~~"
		L"west_�����~--~_������~--~east_��ʯ���~"
		L"~~~~~"
		L"~~~~~");
	SetInfo("10219", 
		L"~~~~~"
		L"~~~~~"
		L"west_������~--~_��ʯ���~--~east_ʯ��ɽ~"
		L"~~~~~"
		L"~~~~~");
	SetInfo("10220", 
		L"~~enter_ʯ����~~~"
		L"~~��~~~"
		L"west_��ʯ���~--~_ʯ��ɽ~~~"
		L"~~~~~"
		L"~~~~~");
	SetInfo("10221", 
		L"~~~~~"
		L"~~~~~"
		L"~~_ʯ����~~~"
		L"~~��~~~"
		L"~~out_ʯ��ɽ~~~");
}