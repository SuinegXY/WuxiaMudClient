#include "stdafx.h"
#include "Map.h"

InstanceRealize(CMapInfo)

CMapInfo::CMapInfo()
{

}

void CMapInfo::init()
{
	SetInfo("01",
		L"~~~~~~(曲阜)~~~~~~~`"
		L"~~~~~~北城门_10104~~酒楼二楼_10116~~~~`"
		L"~~内宅_10123~~~~|~~=~~~~`"
		L"~~|~~春来茶馆_10117~-~北大街_10103~-~醉仙楼_10115~~客房_10139~~`"
		L"后院_10121~-~孙府正厅_10120~-~厨房_10122~~|~~~~=~~`"
		L"~~∧~~盛昌总行_10129~-~北大街_10102~-~-~-~客店_10114~~`"
		L"~~孙府大门_10119~~发呆室_10118~~|~~医铺_10132~~白鹿书院_10134~~`"
		L"~~|~~~∧~|~~|~~|~~`"
		L"西城门_10110~-~西大街_10109~-~西大街_10108~-~中央广场_10101~-~东大街_10111~-~东大街_10112~-~东城门_10113`"
		L"~~|~~~~|~~|~~|~~`"
		L"~~兵营大门_10124~~~~|~~杂货铺_10133~~武馆大门_10136~~`"
		L"~~|~~钱庄_10127~-~南大街_10105~-~当铺_10128~~|~~`"
		L"~~兵营_10125~~~~|~~~~练武场_10137~~`"
		L"~~|~~马车行_10131~-~南大街_10106~-~荣宝斋_10130~~|~~`"
		L"~~兵器库_10126~~~~|~~~~武馆正厅_10138~~`"
		L"~~~~~~南城门_10107~~~~~~`");


	SetInfo("02",
		L"~~~~~~北城门_10208~~~~~~~~~`"
		L"~~~~~~|~~~~~~~~`"
		L"~~~~孔府_10211~-~鼓楼大街_10207~~颜回庙_10217~~~~~~`"
		L"~~~~~~|~~∧~~~~~~`"
		L"~~~~孔庙_10210~-~鼓楼_10206~-~鼓楼大街_10216~~~~~~石门寺_10221`"
		L"~~~~~~|~~|~~~~~~∧`"
		L"西城门_10213~-~西大街_10212~-~西大街_10209~-~鼓楼大街_10205~-~东门大街_10215~-~东城门_10218~-~青石大道_10219~-~石门山_10220`"
		L"~~~~~~|~~~~~~~~`"
		L"~~~~~~鼓楼大街_10204~-~马车行_10214~~~~~~`"
		L"~~~~~~|~~~~~~~~`"
		L"~~~~~~南城门_10203~~~~~~~~`"
		L"~~~~~~|~~~~~~~~`"
		L"~~~~~~青石大道_10202~~~~~~~~`"
		L"~~~~~~|~~~~~~~~`"
		L"~~~~~~青石大道_10201~~~~~~~~`"
		L"~~~~~~(扬州)~~~~~~~~`"
);
}
