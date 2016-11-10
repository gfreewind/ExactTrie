/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  Test routine
 *
 *        Version:  1.0
 *        Created:  10/28/2016 11:14:17 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Gao Feng (gfree.wind@gmail.com) 
 *        Company:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#include "exact_trie.h"

static const char *pattern[] = {
	"test",
	"we",
	"internet",
	"baby",
	"ikuai8",
	"test12",
	"tesa",
	"tesa123",
	"tesa12",
	"xyz",
	"xyz1",
	"xyz2",
	"xyz3",
	"xyz12",
	"xyz13",
	"xyz21",
	"xyz23",
	"xyz123",
	"xyz132",
	
};

static const char *part_pattern[] = {
	"tes",
	"net",
	"ab",
	"yz",
};

static const char *match_str[] = {
	"test",
	"we",
	"internet",
	"baby",
	"ikuai8",
	"tes",
	"net",
	"ab",
	"test12",
	"tesa",
	"tesa123",
	"tesa12",
	"xyz",
	"xyz1",
	"xyz2",
	"xyz3",
	"xyz12",
	"xyz13",
	"xyz21",
	"xyz23",
	"xyz123",
	"xyz132",
};

static const char *cont_match_str = "xyz123";
static int cont_match_times = 4;

static const char *prefix_match_str[] = {
	"test",
	"we",
	"internet",
	"baby",
	"ikuai8",
	"tes",
	"net",
	"ab",
	"test12",
	"tesa",
	"tesa123",
	"tesa12",
	"test1",
	"testa12345",
	"abc",
	"netb",
};

static const char *no_match_str[] = {
	"te",
	"ter",
	"ikuai",
	"hiwe",
	"weare",
	"aby",
	"test123"
};

static const char *prefix_no_match_str[] = {
	"t",
	"e",
	"adfsdfabc",
	"bab",
	"baba1",
};

static const char *g_exact_strs[] = {
	"api.anyview.com",
	"106.3.136.10",
	"res.zilong.37wan.com",
	"res.dtx.game2.com.cn",
	"qisu.video.qiyi.com",
	"v.ku6.com",
	"m.ku6.com",
	"pcvideoyd.titan.mgtv.com",
	"flv.res.hunantv.com",
	"cs.vmoiver.com",
	"dm.pipi.cn",
	"vod3.nty.tv189.cn",
	"pc.youku.com",
	"eshare.live.otvcloud.com",
	"www.zhibo9.cc",
	"www.dasai8.com",
	"113.108.220.52:9111",
	"ipad.cmvideo.cn:8080",
	"migu.cmvideo.cn",
	"vod.gslb.cmvideo.cn",
	"wapx.cmvideo.cn:8080",
	"vod.hcs.cmvideo.cn:8088",
	"gg.ewang.com",
	"tv.cp81.ott.cibntv.net",
	"api.cp33.ott.cibntv.net",
	"pw.live.fangyantianxia.cn",
	"v1.play.fangyantianxia.cn",
	"manhua.haomee.cn",
	"manhua.tmcdn.net",
	"vmag.vivame.cn",
	"interface.vivame.cn",
	"solar.fbcontent.cn",
	"gallery.fbcontent.cn",
	"xyst.yuanfudao.com",
	"apicng.dili360.com",
	"yuntv.letv.com",
	"info.3g.qq.com",
	"bbs.j.cn",
	"api.5wuli.com",
	"et.w.inmobi.com",
	"appservice.114piaowu.com",
	"tracker00.nmenu.cn:6970",
	"115.238.227.152:8080",
	"telemetry.battle.net",
	"cdnres.ddt.qq933.com",
	"miyu.apps.ibaihe.com",
	"image.meibumei.com",
	"vv.video.qq.com",
	"api.ibbpp.com",
	"download.loocha.com.cn",
	"vv.loocha.cn",
	"api.ttyongche.com",
	"edge.ivideo.sina.com.cn",
	"edge.ivideo.sina.com.cn.wscdns.com",
	"api.izhangchu.com",
	"s1.cdn.xiangha.com",
	"jhdp.xhby.net:81",
	"jhdr.xhby.net",
	"rmrbaudio.people.com.cn",
	"yzm.111.com.cn",
	"yzimg.fangkuaiyi.com",
	"api.newad.ifeng.com",
	"adash.m.taobao.com",
	"server.hao24.com:9999",
	"api.m.mi.com",
	"api.share.mob.com:80",
	"iphone.ac.qq.com",
	"static2.ivwen.com",
	"www.imeihuo.cn",
	"www.abaitu.com:54668",
	"uedas.qidian.com",
	"img.nuannuan.5317wan.com",
	"125.39.236.120:8580",
	"special.dajie.com",
	"api.doufu.diaobao.la",
	"img.diaobao.la",
	"api.yh.jianzhimao.com",
	"img.jianzhimao.com",
	"hello.lianaibiji.com",
	"static.lianaibiji.com",
	"app.qiujiaoyou.net",
	"image.static.qiujiaoyou.net",
	"video.static.qiujiaoyou.net",
	"img.sumeme.com",
	"im.memeyule.com:90",
	"ws.memeyule.com:6010",
	"api.qiulianai.cn",
	"imimg.v89.com:8800",
	"img.v89.com",
	"image.mylife.u17t.com",
	"napi.xxsy.net",
	"mobileapi.yuanlai.com",
	"media.tadu.com",
	"androidapi22.tadu.com",
	"image.wufazhuce.com",
	"v3.wufazhuce.com:8000",
	"shophm.9you.com",
	"mst.vip.com",
	"oc.umeng.com",
	"221.130.190.225",
	"amdc.m.taobao.com",
	"wanggoumiaosha.image.alimmdn.com",
	"www.qugou-inc.com",
	"push.mobile.kugou.com",
	"media.store.kugou.com",
	"log.stat.kugou.com",
	"freetyst.mll.migu.cn",
	"a.mll.migu.cn",
	"p.mll.migu.cn:8080",
	"m.ctrip.com",
	"app.cntv.cn",
	"newvector.map.baidu.com",
	"gateway.qiche.tieyou.com",
	"qp.cl0579.com",
	"www.jy5188.com",
	"img1.fj173.net",
	"www.ttyouxi.com",
	"api.dianzhangzhipin.com",
	"client.wap.wxcs.cn",
	"api.mob.app.letv.com",
	"tianqi.2345.com",
	"api.babytree.com",
	"image.diyidan.net",
	"img.khb.ciwong.com",
	"style.ciwong.net",
	"open.lmbang.com",
	"rapi.mama.cn",
	"images.rapi.mama.cn",
	"image-qzone.mamaquan.mama.cn",
	"mapi.mama.cn",
	"static.cosmeapp.com",
	"api.qingchifan.com",
	"ads.wepiao.com",
	"androidcgi.wepiao.com",
	"static.show.wepiao.com",
	"app.jf.10086.cn",
	"clientdispatch.10086.cn:8080",
	"edooon.com",
	"api.icartoons.cn",
	"img0.icartoons.cn",
	"manhua.qpic.cn",
	"android.ac.qq.com",
	"megatron.platform.zongheng.com",
	"mi.zhaopin.com",
	"stat.octinn.com",
	"brup.365shengri.cn",
	"api.9wuli.com",
	"wapiknow.baidu.com",
	"kstj.baidu.com",
	"newlcs.zhidao.baidu.com:80",
	"zhidao.baidu.com",
	"iknow01.bosstatic.bdimg.com",
	"t.nikankan.com",
	"tupian.nikankan.com",
	"api.chuchujie.com",
	"img.cdn.daling.com",
	"passport.daling.com",
	"m.ymall.com",
	"140.207.217.32",
	"114.80.165.113",
	"stat.api.dianping.com",
	"meilapp.qiniucdn.com",
	"api.meilapp.com",
	"ymtlog.ymatou.com",
	"s1.ymatou.com",
	"static.jyh.ymatou.com",
	"app.ymatou.com",
	"api.m.gou.com",
	"mobi.111.com.cn",
	"s.maiyaole.com",
	"nest.111.com.cn",
	"ossgw.alicdn.com",
	"appdownload.alicdn.com",
	"zhuanzhuan.58.com",
	"api.lego.wireless.58.com",
	"tcmobileapi.17usoft.com",
	"tafrtt.map.qq.com",
	"182.254.116.117",
	"mi.gdt.qq.com",
	"ips.ifeng.com",
	"ting.d.ireader.com",
	"m5.file.xiami.com",
	"utop.umengcloud.com",
	"resource.ising.migu.cn",
	"jk.ising.migu.cn",
	"lehu-pic.wcsapi.biz.matocloud.com",
	"md.dama1.cn",
	"game.str.mdt.qq.com:8080",
	"config.tsci.com.cn",
	"iyuedu.qq.com",
	"www.hktvmall.com",
	"grand.ele.me",
	"static.apple.nextmedia.com",
	"hk-nextmedia.videoplaza.tv",
	"logging.apple.nextmedia.com",
	"vtrk.nextmedia.com",
	"web-appledaily-hk-logging.nextmedia.com",
	"optimus.autohome.com.cn",
	"jp.kankan.1kxun.mobi",
	"pc.yingshi.tcclick.1kxun.mobi",
	"p.store.qq.com",
	"www.zybang.com",
	"push.zybang.com:8801",
	"res.51changdu.xyz",
	"api.andreader.xyz",
	"client.cmread.com",
	"changyan.sohu.com",
	"k.sohu.com",
	"tjlog.easou.com:8900",
	"api.easou.com",
	"image.book.easou.com",
	"bookwebview.easou.com",
	"book.2345.com",
	"link.52itv.cn",
	"stat.52itv.cn",
	"mytv.tvb.com",
	"img.beevideo.tv",
	"go.10086.cn",
	"api.iapps.ifeng.com",
	"c1.ifengimg.com",
	"stats.yzbo.tv",
	"api.tv.sohu.com",
	"wmhls.bn.netease.com",
	"newsapi.sina.cn",
	"cctv13.vtime.cntv.wscdns.com",
	"v1.go2yd.com",
	"diandiancdn.cdvcloud.com",
	"alcdn.img.xiaoka.tv",
	"wscdn.yixia.com",
	"pzi.haotougu.com",
	"simu.niugubao.com:8000",
	"xrz-bbs.img-cn-hangzhou.aliyuncs.com",
	"plus.app.baihe.com",
	"click.baihe.com",
	"bingo.boohee.com",
	"kwflvcdn.000dn.com",
	"staticlive.douyucdn.cn",
	"www.ikuaiyue.com",
	"shipin.yundzh.com",
	"www.gw.com.cn",
	"cimage.gw.com.cn",
	"rebo.yundzh.com",
	"da.mmarket.com",
	"conn.gf.com.cn:80",
	"tx-pns.gf.com.cn",
	"blog.guosen.com.cn",
	"tkdata.guosen.cn",
	"goldsunhq1.guosen.cn:8004",
	"a1.nicaifu.com",
	"item.ttkvod.com:8081",
	"180.96.8.44",
	"www.windpartner.com.cn",
	"v3.dandanlicai.com",
	"venus.jd.com",
	"cdnware.m.jd.com",
	"112.124.27.71:8383",
	"api.touker.com",
	"www.sse.com.cn",
	"mb.sseinfo.com",
	"yunhq.sse.com.cn:32041",
	"sns.sseinfo.com",
	"bsy.qiniu.vmovier.vmoiver.com",
	"m.1hai.cn",
	"ourtimespicture.bs2dl.yy.com",
	"hdl3a.douyucdn.cn",
	"staticlive.douyutv.com",
	"rpic.douyucdn.cn",
	"shark.douyucdn.cn",
	"dotcounter.douyutv.com",
	"img.dwstatic.com",
	"img.lolbox.duowan.com",
	"www.ffpic.net",
	"vpic.easou.com",
	"og1.molitv.cn:9312",
	"ad.molitv.cn",
	"img.tv.moliv.cn",
	"api.yiqi1717.com",
	"yiqi1717-10009627.image.myqcloud.com",
	"alcdn.hls.xiaoka.tv",
	"image.scale.inke.com",
	"121.18.168.56",
	"serviceinfo.inke.tv",
	"imgstbox.voole.com",
	"mobileservice.voole.com:35122",
	"webapp.yoosee.cc",
	"dm.xiazaibao.xunlei.com",
	"lu.srv.xzb.xunlei.com",
	"upgrade.xiazaibao.xunlei.com",
	"yc.xiazaibao.xunlei.com:8182",
	"bsy.qiniu.vmagic.vmoiver.com",
	"videoimage1.cutv.com",
	"sns.cutv.com",
	"video.ted.com",
	"hls.npr.org",
	"img3.tv189.cn",
	"pic01.v.vnet.mobi",
	"img.mms.v1.cn",
	"f04.v1.cn",
	"img.app.v1.cn",
	"dc.v1.cn",
	"api.moliv.cn",
	"avatar.graphmovie.com",
	"rom.16tree.com",
	"nowtv.tvesou.com",
	"tvnow-pic.tvesou.com",
	"match.p4p.1688.com",
	"hm.baidu.com",
	"irs01.com",
	"i.mgtv.com",
	"pic.ys7.com:99",
	"piccover.ys7.com",
	"vector0.map.bdimg.com",
	"client.map.baidu.com",
	"api.ppzuche.com",
	"cdn.ppzuche.cn",
	"youtx.3g.fang.com",
	"www.roadqu.com",
	"vstlog.17usoft.com",
	"app.xinggenet.com",
	"120.27.194.54",
	"pic2.40017.cn",
	"qbwup.imtt.qq.com",
	"tgccfg.qq.com",
	"dlied1.qq.com",
	"webs.tgp.qq.com",
	"cdn.tgp.qq.com",
	"pingjs.qq.com",
	"cod.tgp.qq.com",
	"mediacloud01.doapi.net",
	"www.ruoogle.com",
	"android.piaoliupingzi.com:6666",
	"videohy.tc.qq.com",
	"ugc.qpic.cn",
	"oth.eve.mdt.qq.com:8080",
	"mediags.moretv.com.cn",
	"static.tvmao.com",
	"serveraddr.serviceweb.kugou.com",
	"x.pps.tv",
	"glider.xiu.pps.tv",
	"www.imifun.com",
	"tui.9see.com",
	"www.akslsndlknq.com",
	"s.mmbroadcast.net",
	"www.videohupu.com",
	"img.app.meitudata.com",
	"www.meipai.com",
	"www.woxiu.com",
	"www.zhibo.tv",
	"user.tvesou.com",
	"live.bilibili.com",
	"material.mediav.com",
	"clientapi.wasu.cn",
	"www.qie.tv",
	"qf.56.com",
	"yuzijiang.qsyy.ks3-cn-beijing.ksyun.com",
	"live.img.52ytv.cn",
	"live.cc.52ytv.cn",
	"page.amap.com",
	"trafficapp.autonavi.com:8888",
	"assets.adobedtm.com",
	"touch.qunar.com",
	"vliveachy.tc.qq.com",
	"cmp3.o2ting.com:8081",
	"bcs.kuaiapk.com",
	"video.cdn.baidupcs.com",
	"www.fun.tv",
	"gd.qq.com",
	"pay.qq.com",
	"wvideo.spriteapp.cn",
	"live.cntv.cn",
	"player.cntv.cloudcdn.net:81",
	"ip.apps.cntv.cn",
	"cbox.cntv.cn",
	"img.cctvpic.com",
	"asp.cntv.lxdns.com",
	"api.cntv.cn",
	"m.mapps.m1905.cn",
	"m3u8i.vodfile.m1905.com",
	"hot.vrs.sohu.com",
	"dl.ijinshan.com",
	"picture.mxhapp.com",
	"128k-qiniu.tingall.com",
	"service.taobao.com",
	"113.31.65.232:8080",
	"113.31.65.228:2011",
	"file.qianqian.com",
	"data.vdisk.me",
	"update.zmrgame.com",
	"main.gslb.ku6.com",
	"www.jjvcd.com",
	"login.tudou.com",
	"data.video.qiyi.com",
	"update.my.91.com",
	"user.qzone.qq.com",
	"tvbar.uusee.com",
	"player.uusee.com",
	"vagaa.com",
	"86mms.megajoy.com",
	"pay.file2.joy.ccgslb.com.cn",
	"adslvfile.qq.com",
	"passport.sohu.com",
	"www.eyou.com",
	"web.minigame.xunlei.com",
	"metan.video.qiyi.com",
	"vod.maxtv.cn",
	"qipai.doshow365.com",
	"idx.3g.yy.com",
	"update.yy.duowan.com",
	"mail.google.com",
	"pad.kankan.com",
	"phone.xunlei.com",
	"download.pplive.com",
	"api.login.icq.net",
	"wenku.baidu.com",
	"download.cdn.mozilla.net",
	"mail.tom.com",
	"vip.tom.com",
	"cmail.tom.com",
	"mail.hexun.com",
	"mail.sogou.com",
	"mail.xinhuanet.com",
	"mail.eastday.com",
	"reg.rednet.cn",
	"rescdn.qqmail.com",
	"mail.qq.com",
	"www.shangmail.com",
	"mail.china.com",
	"dcdn.kugou.com",
	"mail.10086.cn",
	"downloads.chinagames.net",
	"xmp.kankan.xunlei.com",
	"upload.vdisk.me",
	"masterconn.qq.com",
	"mini.xiu5.com",
	"service.kkyoo.com",
	"www.kkyoo.com",
	"www.loveliao.com",
	"babelfish.yahoo.com",
	"api.microsofttranslator.com",
	"honyaku.yahoo.co.jp",
	"www.systranet.com",
	"www.reverso.net",
	"www.excite.co.jp",
	"www3.j-server.com",
	"www.lec.com",
	"www.poltran.com",
	"fanyi.baidu.com",
	"fanyi.youdao.com",
	"webim.jiayuan.com",
	"www.jiayuan.com",
	"video.mywtv.cn",
	"jh.01lm.com",
	"audio.vojs.cn:8000",
	"www.vojs.cn",
	"data.vod.itc.cn",
	"update.lingxiu98.com",
	"upgrade.91u.91.com",
	"dii.cn",
	"down.coco360.com",
	"interface.coco360.com",
	"im1.cq.vg",
	"www.manhuacheng.com",
	"client.doyo.cn",
	"www.douban.com",
	"www.duba.net",
	"stat.ijinshan.com",
	"sample-receive.master.qq.com",
	"media.xinhuanet.com",
	"zhangmenshiting.baidu.com",
	"qn.gph.netease.com",
	"update.qn.163.com",
	"update.tcjdyou.com",
	"user.jdyou.com",
	"pcgame.7kbox.com",
	"box.7k7k.com",
	"flash.7k7k.com",
	"login.7k7k.com",
	"hz.qqdisk.ftn.qq.com",
	"nbxml.xl7.sandai.net",
	"jsq.xunlei.com",
	"gdl0402.linekong.com",
	"xy.linekong.com",
	"dl.client.baidu.com",
	"softdown.rising.net.cn",
	"pc.rising.cn",
	"9z.szgla.cn",
	"update.tl.sohu.com",
	"login.360.cn",
	"update.worldoftanks.cn",
	"listen.ph.126.net",
	"sssg.wan.360.cn",
	"sgk.xdcdn.net",
	"cloud.ebox.163.com",
	"upload.cloud.ebox.163.com",
	"www.game161.com",
	"update.bfyx.net",
	"download.91mq.com",
	"chat.178cpw.com",
	"www.94kuai.com",
	"www.paopaogame.net",
	"qp.057718.com",
	"sip.anycall.com.hk",
	"phone.66call.net",
	"dx.cr173.com",
	"game.yxyun.com",
	"download.go7744.com",
	"class.yeshj.com",
	"down.u8i8.net",
	"upload.mvbox.cn",
	"qqbaby.qq.com",
	"v.iseeyoo.cn",
	"game.27.cn",
	"www.youxigongshe.com",
	"www.youxitt.com",
	"download.51.com",
	"www.caihong.com",
	"dls1.gc73.com",
	"lobby.gc73.com",
	"api.61.com",
	"appsvc.kuaiwan.com",
	"games.qq.com",
	"client.skype.tom.com",
	"vdisk.me",
	"download.5211game.com",
	"pro.api.115.com",
	"qqmusic.qq.com",
	"t.rednet.cn",
	"vcity.yhgame.cn",
	"passport.cga.com.cn",
	"login.sina.com.cn",
	"m0.mail.sina.com.cn",
	"pay.yhgame.cn",
	"g2.mail.10086.cn",
	"bl.qq.com",
	"socres.smggame.net",
	"shangyouhd.dnion.com",
	"vpn.my18.com.cn",
	"gcktv.51mike.com",
	"play.m12.djcc.com",
	"ufaapi.kuaipan.cn",
	"kad.k.wps.cn",
	"dlsw.baidu.com",
	"b.cdn.baidu.com",
	"asimgs.pplive.cn",
	"r.03.sycdn.kuwo.cn",
	"shouji.baofeng.com",
	"t13hz.mail.163.com",
	"pgdt.gtimg.cn",
	"wx.qlogo.cn",
	"weixin.qq.com",
	"wx.qq.com",
	"web.sanguosha.com",
	"cv.6rooms.com",
	"jsbc.dnion.com",
	"download.weipan.cn",
	"dfs.kuaipan.cn:8080",
	"vhotws.video.qq.com",
	"up.sinastorage.com",
	"www.speedtest.net",
	"imge.kugou.com",
	"netsp.master.qq.com",
	"s.x.baidu.com",
	"sendfile.vip.xunlei.com:8585",
	"pcvideo.imgo.tv",
	"photocdn.sohu.com",
	"interface.kugou.com",
	"qd.bs.baidu.com",
	"www.ouou.com",
	"api.songlist.ttpod.com",
	"img.cache.itlily.com",
	"c20.aipai.com",
	"vip.xunlei.com",
	"web.stat.ws.126.net",
	"p-log.ykimg.com",
	"CN.depot.battle.net:1119",
	"wfqqreader.3g.qq.com",
	"updatecenter.qq.com",
	"live.yy.com",
	"update.7fgame.com",
	"game.ds.qq.com",
	"mimg.127.net",
	"mail.126.com",
	"mail.sohu.com",
	"mail.yeah.net",
	"mail.163.com",
	"download.wegame.qq.com",
	"img.xiami.net",
	"bfas.bianfeng.com",
	"assetcdn.111.cgw2.com",
	"gapi1.yunpan.360.cn",
	"imapi.5211game.com:3080",
	"video.bcloud.brtn.cn",
	"api.changba.com",
	"changba.gslb.letv.com",
	"mail.21cn.com",
	"wpc.250f.edgecastcdn.net",
	"dl.softmgr.qq.com",
	"pcs.baidu.com",
	"tj.ctfs.ftn.qq.com",
	"sh.vnet.cn",
	"www.hao123.com",
	"asxy.3g.qq.com",
	"iir.blizzard.com:3724",
	"download.microsoft.com",
	"dasoft.9553.com",
	"pl.youku.com",
	"proxy.music.qq.com",
	"stat.hujiaozhuanyi.com",
	"mobi.kuwo.cn",
	"iface.iqiyi.com",
	"api.m.56.com",
	"coral-topic.u.qiniudn.com",
	"games.mobileapi.hupu.com",
	"fdf.7958.com",
	"www.ikuaishou.com",
	"app.video.baidu.com",
	"v.img.pplive.cn",
	"tips.isp.pptv.com",
	"jsonfe.funshion.com",
	"media.v.xunlei.com",
	"pgv.m.vip.xunlei.com:8090",
	"api.cloud.v.xunlei.com:8080",
	"in.3b2o.com",
	"friend.uxin.com:19198",
	"api.uxin.com",
	"commbook.uxin.com:9098",
	"im.uxin.com:8887",
	"analytics.zhihu.com",
	"www.zhihu.com",
	"p1.zhimg.com",
	"112.124.52.172",
	"line.naver.jp",
	"phobos.apple.com",
	"openbox.mobilem.360.cn",
	"cdn.baidupcs.com",
	"s9032.mobile01.qins.com",
	"www.attagame.cn",
	"www.c1wan.com",
	"stat.attagame.cn",
	"m2.music.126.net",
	"cgi.connect.qq.com",
	"wap.uuwldh.com",
	"cdn.lizhi.fm",
	"zhqhcheck.weebia.com",
	"tiantian.xincgames.com",
	"log.laohu.com",
	"cqsl.linekong.cn",
	"gdl2301.linekong.com",
	"118.26.239.22:80",
	"chinameeting365.com",
	"file.yungengxin.com",
	"mobilecdn.kugou.com",
	"service.kugou.com",
	"183.60.15.153",
	"dispatch.tc.qq.com",
	"qqvideo.tc.qq.com",
	"pic.feiji.qq.com",
	"aqcenter.taobao.com",
	"wxapi.taobao.com",
	"t0.qlogo.cn",
	"tcli.3g.qq.com",
	"r.t.qq.com",
	"xc.youja.cn",
	"uplus.dpocket.cn",
	"58.68.229.242",
	"bbm.brtn.cn",
	"amonsul.douban.com",
	"api.m.renren.com",
	"image.crossmo.com",
	"webkitui.youyuan.com",
	"pd.youyuan.com",
	"www.yqkan.cn",
	"gnews.yqkan.cn",
	"img.yqkan.cn",
	"210.14.146.123:3014",
	"pic.qiushibaike.com",
	"msg.qiushibaike.com",
	"mat1.gtimg.com",
	"wsi.qq.com",
	"www.voyager-app.com:8088",
	"restapi.amap.com",
	"www.imxingzhe.com",
	"img.voyager-app.com",
	"c.tieba.baidu.com",
	"tieba.baidu.com",
	"im.tieba.baidu.com:8000",
	"tb.himg.baidu.com",
	"api.kbvoice.com",
	"mimi.wumii.cn",
	"mim520.sinaapp.com",
	"218.11.178.165:80",
	"blued.u.qiniudn.com",
	"y.sdo.com",
	"www.inbilin.com",
	"img-bilin.qiniudn.com",
	"static.wecomics.cn",
	"up.qiniu.com",
	"vip.laiwang.com",
	"images.travelnow.com",
	"mm.amap.com:80",
	"webfiles.amap.com",
	"mps.amap.com",
	"sns.amap.com",
	"poi.amap.com",
	"oss.amap.com",
	"aos-cdn-image.amap.com",
	"service.anav.com",
	"211.137.35.180",
	"images.movie.xunlei.com",
	"smart.789.image.mucang.cn",
	"file.aliyun.kakamobi.cn",
	"api.sijimishu.com",
	"car.xiaojukeji.com",
	"omgup.xiaojukeji.com",
	"as.xiaojukeji.com",
	"seraph-api.xiaojukeji.com",
	"m.variflight.com",
	"cdn.feeyo.com",
	"www.kldlk.com",
	"mc.aibang.com",
	"p.longtugame.com",
	"yht.morefuntek.com",
	"dl.junwang2.com",
	"szrsl.linekong.cn",
	"14.17.123.242:80",
	"img.jiaowomj.com",
	"a.anxin0525.com",
	"api.imwuya.com",
	"117.135.163.146",
	"pes.huying.info:9999",
	"www.talkingflower.com",
	"trip.taobao.com",
	"42.156.166.50",
	"42.120.111.79:8003",
	"yizhouyou.51you.com",
	"img.xhlx.cn",
	"www.weidaijia.cn",
	"breadtripimages.qiniudn.com",
	"customer.vvipone.com",
	"m.mobvoi.com:55555",
	"heartbeat.mobvoi.com",
	"www.aapinche.cn",
	"trip.163.com",
	"map.qq.com",
	"indoorloc.map.qq.com:8080",
	"shmain.fcloud.store.qq.com",
	"v.p2psearcher.org",
	"wlanwm.12530.com",
	"res.dts.37wan.com",
	"bookpic.u.qiniudn.com",
	"moli.autopatch.173.com",
	"dzmasset.playcrab.com",
	"appstore.renjiang.playcrab.com",
	"1251021643.cdn.myqcloud.com",
	"banner.lohaa.com",
	"sdk.rekoo.net:8080",
	"www.sinaimg.cn",
	"api.139talk.com",
	"a.cytxl.com.cn",
	"data.haoma.sogou.com",
	"wap.keepc.com",
	"godfs.3g.cn",
	"113.31.65.232",
	"ws.365anycall.cn",
	"dntg.moqikaka.com",
	"tdzm.hugenstar.org",
	"mc.fysg.koramgame.com",
	"dl.duoyi.com",
	"m2swupdate.duoyi.com",
	"cnxb.ppgame.com:6005",
	"122.112.3.177",
	"bjchina-dl.glu.com",
	"ygame.yilegame.com",
	"114.215.137.236",
	"42.121.236.127",
	"54.251.171.70",
	"v.youku.com",
	"mobileapp.app27571.twsapp.com",
	"61.130.247.189",
	"cmp3.o2ting.com",
	"113.31.65.229",
	"113.31.65.226",
	"group.ye.qq.com",
	"img.25pp.com",
	"ucdl.25pp.com",
	"cdn.jzwc.yaowan.com",
	"yunpan.360.cn",
	"f.qcloudcos.com",
	"images.71ao.cn",
	"api.xiangyou.renren.com",
	"live.showself.com",
	"117.135.171.72",
	"gwkl.moogame.cn",
	"sdk.api.kunlun.com",
	"xldown.qq.com",
	"182.254.184.233:8010",
	"42.62.46.151",
	"a.atonghua.net",
	"wap.shengqianliao.com",
	"113.31.65.228",
	"updatedudu.ztgame.com.cn",
	"gaios.talkingdata.net",
	"hijoyusers.joymeng.com:",
	"api.sina.cn",
	"reader.qq.com",
	"inews.gtimg.com",
	"app.nuomi.com",
	"blmobile.3g.qq.com",
	"app1000000129.twsapp.com",
	"android.qiaiapp.com",
	"yimg.nos.netease.com",
	"laipa.oss-cn-hangzhou.aliyuncs.com",
	"42.121.29.240",
	"oss.aliyuncs.com",
	"phoneplus.langma.cn",
	"public.langma.cn",
	"api.yuanfen5.com",
	"data.iapppay.com:8082",
	"pan.baidu.com",
	"res.qm.g.yx-g.cn",
	"m.taobao.com",
	"pingma.qq.com",
	"dgf.ikamobile.cn",
	"116.228.55.13:8107",
	"xstm.v.360.cn",
	"api.m.v.360.cn",
	"live-ios.gamefy.cn",
	"42.96.196.180",
	"epg.tvesou.com",
	"182.92.189.193",
	"42.121.111.105",
	"video.tvowow.com",
	"vapp.fw.live.cntv.cn",
	"gamepro.qiniudn.com",
	"union.innmall.cn",
	"res.cocounion.com",
	"live.itv.doplive.com.cn",
	"tvbtest.ctrmi.com",
	"112.124.32.9",
	"staticnova.ruoogle.com",
	"app.scity.cn",
	"www.xiaojiangyou.com",
	"ezshenghuo.qiniudn.com",
	"www.mhao.me",
	"mv1.100tv.com",
	"wsm3u8live.imgo.tv",
	"live.3gv.ifeng.com",
	"static.adwo.com",
	"hoapi.gomumu.mobi",
	"gomumu.com.cn",
	"42.62.24.112",
	"cgi.kg.qq.com",
	"banner.img.static.youmi.net",
	"fast.yingyonghui.com",
	"n0.itc.cn",
	"thumbnail-cdn.peiwo.me",
	"bottlefile.oss-cn-hangzhou.aliyuncs.com",
	"www.52plp.com:6666",
	"img.talk.kakao.co.kr",
	"agw.weishuo.cn:4009",
	"im.guoling.com:2011",
	"mobile.hcsql.com",
	"ldxy.163.com",
	"service.game100.cn",
	"xym.gph.netease.com",
	"xym.163.com",
	"smzt.qq.com",
	"sdk.ffcai.com:8910",
	"update.u58.com",
	"wifi.360.cn",
	"netmon.stat.360safe.com",
	"intf1.zsall.mobilem.360.cn",
	"short.wecall.qq.com",
	"ahero.login1.61.com",
	"mcdn.61.com",
	"yfapi.daotongdao.com",
	"mobilephone.efenliao.com:2011",
	"www.huiian.com:8080",
	"kelu.oss-cn-hangzhou.aliyuncs.com",
	"zxandroid1.chinaim365.com:6077",
	"zsbsnsandroidu1.chinaim365.com:7099",
	"api7.mobile.lashou.com",
	"mobile.yougou.com",
	"cfg.cml.ksmobile.com",
	"mapi.lingduohome.com",
	"zb.allook.cn:80",
	"mpv.videocc.net",
	"vhoth.dnion.videocdn.qq.com",
	"dlvod.cdn.zhanqi.tv",
	"api.miyue.cc",
	"res.smzdm.com",
	"logger.qingting.fm",
	"qingting.u.qiniudn.com",
	"dl.br.baidu.com",
	"p1.img.cctvpic.com",
	"mobile.sports.cntv.cn",
	"video.mukewang.com",
	"duanzi.duowan.com",
	"42.62.77.241:8080",
	"files.mchang.cn",
	"58.30.32.136:8090",
	"tedhls-vod.hls.adaptive.level3.net",
	"hls.ted.com",
	"netbar.qq.com",
	"ups.qq.com",
	"1000.ttlz.qq.com",
	"version.knights.qq.com",
	"zt.3gpk.ne",
	"ck3-ios.lb.prodcn.gluon.glu.com",
	"app.shopapi.xiaomi.com",
	"pingan.com.cn",
	"stock.pingan.com",
	"pad-appdata.pingan.com.cn",
	"blink.naonaola.com",
	"www.waitta.com",
	"dating.apps.ibaihe.com",
	"images.ibaihe.com",
	"ahlmdl.autopatch.173.com",
	"res.xianjian.qq.com",
	"140.207.69.120:10001",
	"yzr.163.com",
	"update.yzr.163.com",
	"d4xyptu3bni30.cloudfront.net",
	"54.218.85.166:9999",
	"adpdcs.4399sy.com",
	"dota.sso.taiqigame.com",
	"dota.patch.taiqigame.com",
	"udata.taiqigame.com",
	"interface.sj.pook.com",
	"ahzs-cdnres.me4399.com",
	"s310.ahzs.4399sy.com",
	"bootstrap.tango.me:8080",
	"feed.tango.me",
	"css.baojia.com",
	"api.baojia.com",
	"pic.baojia.com",
	"webservice.erlinyou.com",
	"mdownload.erlinyou.com",
	"i.apps.sina.cn",
	"api.woouqu.com",
	"static.woouqu.com",
	"api.anjuke.com",
	"pic1.ajkimg.com",
	"urltracker.yunfan.com:8081",
	"app.koochat.com",
	"koochat.duapp.com",
	"cdn.cycs.9377.com",
	"fyj.wanzhuapp.com",
	"nga.178.com",
	"ma.3g.qq.com:80",
	"img.pindou.com",
	"wap.dl.pinyin.sogou.com",
	"api.snacks.pindou.com",
	"api.hcicloud.com",
	"downlogo1.yy.duowan.com",
	"act1.m.yystatic.com",
	"m.yy.com",
	"hp.proxy.yy.duowan.com:8080",
	"act.3g.yy.com",
	"alimp3.changba.com",
	"v1.ard.q.itlily.com",
	"img.17u.cn",
	"mpp.taobao.com",
	"www.153.cn:8081",
	"pic.153.cn:8081",
	"img.153.cn:49992",
	"pic2.58cdn.com.cn",
	"api.hotelvp.com",
	"hotelvp.qiniudn.com",
	"api.lanrenzhoumo.com",
	"image.mibang.meishixing.com",
	"img.uuzuche.com.cn",
	"com.zhizunapp.useca",
	"t.35.com",
	"mail.china-channel.com",
	"push.35.com:5222",
	"mial.35.cn",
	"mi.95195.com",
	"cc.mmlove.mmlove",
	"fm.ifengcdn.com",
	"video19.ifeng.com",
	"fm.ifeng.com",
	"imgs.diantai.ifeng.com",
	"diantai.ifeng.com",
	"fm.shuoba.org",
	"dr.kugou.net",
	"policy.video.iqiyi.com",
	"qc.m.tv.sohu.com",
	"mobile.ximalaya.com",
	"media.kxting.cn",
	"lyric.kxt.duomi.com:80",
	"agw.4gdh.net",
	"t.4gdh.net",
	"img.chengmi.com",
	"api.chengmi.com",
	"jr-user.qiniudn.com",
	"app.ijianren.com:9898",
	"api.weilver.com",
	"pic.weilver.com",
	"app.dragonpass.com.cn",
	"www.immeiya.com",
	"telecom.dota2.wmsj.cn",
	"cdn.dota2.com.cn",
	"unicom.dota2.wmsj.cn",
	"echoapi.kibey.com",
	"api.yimiwork.com",
	"yimiimg.qiniudn.com",
	"ktvme.qiniudn.com",
	"www.ktvme.com",
	"tm.vvvdj.com",
	"hd.mail.189.cn",
	"api.mail.189.cn",
	"yixin.dl.126.net",
	"api.kongjianbao.cn",
	"m.qzone.com",
	"api.dianhua.dianxin.net",
	"mcloud.kingdee.com",
	"api.peiyinxiu.com",
	"yxbwbsc.tv189.com",
	"update.txhd.163.com",
	"mig.com.cn",
	"ios.uhero.cc",
	"118.192.72.25:5000",
	"stat.uu.cc",
	"login.xjcard.qq.com",
	"ts.eset.com",
	"alive.rbc.cn:80",
	"d1i6vahw24eb07.cloudfront.net",
	"anime.m.kankan.com",
	"androidphone.stat.kankan.com",
	"inf.phonmedia.com",
	"up.ninebox.cn",
	"www.verycd.com",
	"i-7.vcimg.com",
	"www.longlongtv.com",
	"xml.kanketv.com",
	"client.kanketv.com",
	"d.kanketv.com",
	"ser3.graphmovie.com",
	"static.storm8.com",
	"restaurant.teamlava.com",
	"qq.tz.qq.com:18002",
	"ws.idouniu.com",
	"qq.fn.qq.com",
	"app900000926.twsapp.com",
	"apipub-ytios.gz.1251007284.clb.myqcloud.com",
	"game.ye.qq.com",
	"hzone.ye.qq.com",
	"ugc.sun-cam.com.cn",
	"ugc.cutv.com",
	"mobileapp.roadqu.com",
	"ms.shijiebang.com",
	"mapi.uzai.com",
	"www.australiaplus.com.au",
	"www.radioaustralia.net.au",
	"com.kuxun.scliang.travel",
	"api.m.kuxun.cn",
	"www.zhenrenyi.com",
	"img.517huwai.com",
	"www.shunfengdj.cn",
	"ssapp.40000km.com.cn",
	"igogo.enavi.189.cn",
	"crmroam.qq.com",
	"api.liwushuo.com",
	"adash.m.toabao.com",
	"liwushuo-data.qiniudn.com",
	"fittingroom.doov.com.cn",
	"mobile.banggo.com",
	"mobile.gome.com.cn",
	"android.secoo.com",
	"static.g.pptv.com",
	"data.3g.yy.com",
	"quan.m.xunlei.com",
	"m.xunlei.com",
	"sm.domobcdn.com",
	"live01.hebtv.com",
	"live.hdpfans.com",
	"wx.tz.qq.com:18001",
	"cdn.hxjy.3304399.net",
	"i.okaybuy.cn",
	"platform.okbuy.com",
	"mobile.hao24.cn",
	"touch.bookingtee.com",
	"www.funguide.com.cn",
	"res.mir.37wan.com",
	"mov.bn.netease.com",
	"ggs.myzaker.com",
	"112.124.96.190:9090",
	"wap.3gwldh.com",
	"si.cc369.cn",
	"agw.ibbzg.com",
	"www.italk24.com",
	"s.dianhua.cn",
	"apis.dianhua.cn",
	"app.gekgek.com",
	"talkandroidv1.chinameeting365.com",
	"www.huipinzhe.com",
	"app.wanhui.cn",
	"res1.wanhui.cn",
	"www.boqii.com",
	"img.boqiicdn.com",
	"vet.boqii.com",
	"www.appzhong.com",
	"p2pmid.baofeng.com",
	"iphone.shouji.baofeng.com",
	"autopatchs1.yulgang2.cdcgames.net",
	"rio.6rooms.com",
	"miner.game.yy.com",
	"ifacelog.iqiyi.com",
	"cache.video.qiyi.com",
	"cache.m.iqiyi.com",
	"downmobile.kugou.com",
	"m.beacon.sina.com.cn",
	"lx.cdn.baidupcs.com",
	"mapi.yhd.com",
	"tracker.yhd.com",
	"lib.wap.zol.com.cn",
	"ads-cdn.chuchujie.com",
	"hulu.youyuan.com",
	"pt.youyuan.com",
	"bbs.daoxila.com",
	"i.dxlfile.com",
	"gc.hvip66.com",
	"dev.open.aiba.com",
	"image.aiba.com",
	"avatar.l99.com",
	"photo.l99.com",
	"web.anyunbao.cn",
	"intf.anyunbao.cn:8881",
	"ywz.hodanet.com",
	"loc.ecplive.cn",
	"115.239.133.244",
	"xavatar.imedao.com",
	"api.51jieyuan.cn",
	"pkimg.image.alimmdn.com",
	"api2.pianke.me",
	"qclx.ipingke.com",
	"user.qianbao666.com",
	"im.qbao.com",
	"yuanfen-production.oss-cn-hangzhou.aliyuncs.com",
	"img.quxiu8.com",
	"weipai-videos.oss-cn-hangzhou.aliyuncs.com",
	"aoya.100bt.com",
	"pic.d3.com.cn",
	"api.d3.com.cn",
	"course.myfriday.cn",
	"qiniu.myfriday.cn",
	"super.u.qiniudn.com",
	"codoon-teiba.codoon.com",
	"api.hudongba.mobi",
	"api.shijia.kakamobi.com",
	"123.kakamobi.cn",
	"api.zapyamobile.net",
	"downloadc.dewmobile.net",
	"netchat.aini25.com",
	"ncimages.oss-cn-hangzhou.aliyuncs.com",
	"m.mama100.com",
	"m200.qiniudn.com",
	"api.ilianmeng.com",
	"img.ilianmeng.com",
	"push.ilianmeng.com",
	"static.etouch.cn",
	"zhwnlapi.etouch.cn",
	"58.gyyx.cn",
	"s.gyyx.cn",
	"api.dongqiudi.com",
	"img.dongqiudi.com",
	"api.social-language.com",
	"7bv8v8.com3.z0.glb.qiniucdn.com",
	"api.zhuizhuiyoyo.com",
	"img.appd.lengxiaohua.cn",
	"a.intsig.net",
	"sex.guokr.com",
	"m.topitme.com",
	"211.155.84.158",
	"www.fenfenriji.com",
	"img.fenfenriji.com",
	"assets.115.com",
	"msg.115.com",
	"passport.115.com",
	"img.91vst.com",
	"play.91vst.com",
	"url.52itv.cn",
	"nj.t.bcsp2p.baidu.com",
	"mimg.moretv.com.cn",
	"vod.moretv.com.cn",
	"lib.cdn.weipan.cn",
	"minigame.qq.com",
	"qqgame.store.qq.com:80",
	"wxtest.bh.173.com",
	"wyphs.sincetimes.com",
	"www.400gb.com",
	"img.tv.moliv.cnn",
	"3gimg.qq.com",
	"i.gtimg.cn",
	"epg.api.pptv.com",
	"qzonestyle.gtimg.cn",
	"bcscdn.baidu.com",
	"gdown.baidu.com",
	"timg.baidu.com",
	"cache.k.sohu.com",
	"vpic.video.qq.com",
	"u1.tdimg.com",
	"udb.duowan.com",
	"udb.yy.com",
	"udb.kuaikuai.cn",
	"img.live.yy.com",
	"k.youku.com",
	"sf.video.qiyi.com",
	"livew.l.qq.com",
	"p2s.cl.kankan.com",
	"v.iask.com",
	"edge.v.iask.com.lxdns.com",
	"retry.r.56.com",
	"zsjcdn.moloong.com",
	"c.isdspeed.qq.com",
	"loading.baofeng5.baofeng.net",
	"box.bfimg.com",
	"log.dc.cn.happyelements.com",
	"etlog.happyelements.cn",
	"static.manimal.happyelements.cn",
	"animalmobile.happyelements.cn",
	"gcn.happyelements.cn",
	"p.yunva.com",
	"actsdk.idreamsky.com",
	"121.201.10.13",
	"game.subway.uu.cc",
	"qypy.qi1game.com",
	"stat.ak47ids.com",
	"360.templerun.uu.cc",
	"musics.wandoujia.com",
	"data.bi.chukong-inc.com",
	"211.152.2.49",
	"121.40.84.160",
	"unionsdk.m.baidu-mgame.com",
	"221.130.10.115",
	"58.67.198.108",
	"sdk.syserver.com.cn",
	"cut.qumi.com",
	"s248.cache.fightpic.com",
	"yichuadserver.oss-cn-hangzhou.aliyuncs.com",
	"dl.vip.yy.com",
	"ydmjzr.changyou.com",
	"pmpush.crimoon.net",
	"image.hydra.qq.com",
	"web.gz.1251006671.clb.myqcloud.com",
	"worldship.sincetimes.com",
	"api.wandoujia.com",
	"xianjian.cmge.center.youkia.net",
	"tjb.xianjian.cmge.center.youkia.com",
	"cloud.189.cn",
	"upload.cloud.189.cn",
	"act.ws.sdo.com",
	"lz.syyx.com",
	"api.m.taobao.com",
	"sdkinit.taobao.com",
	"img.haimi.com",
	"mh.kangxihui.com",
	"pic1.taocz.cnm",
	"m.taocz.com",
	"img.mkf360.com",
	"api.mkf.com",
	"moneymarket.feidee.com",
	"www.qie-zi.com",
	"api-stagging.uwang.me",
	"nm2189cloud.oos-nm2.ctyunapi.cn",
	"static.e.189.cn",
	"open.e.189.cn",
	"dl.url.cn",
	"www.sf-express.com",
	"www.looyu.com",
	"ws.fsmeeting.com",
	"upic.7day.qq.com",
	"kk.91kbox.com",
	"www.chinagames.net",
	"popo.dl.126.net",
	"download1.m3guo.com",
	"tongji.huomaotv.com",
	"live-dl-hls.huomaotv.com",
	"www.huomaotv.com",
	"clientvod.wasu.cn:80",
	"api.meitu.com",
	"fx.kgimg.com",
	"cdn.lepaistudio.com",
	"storage.jd.com",
	"szmail.tfs.ftn.qq.com",
	"ctfs.ftn.qq.com",
	"sh.ctfs.ftn.qq.com",
	"ios-apk-asp.jyw8.com",
	"hqg.webscache.g.pps.tv",
	"pc5.sj.91.com",
	"magic.ucloud.com.cn",
	"nc-5.mg.lightonus.com",
	"static.dingtalk.com",
	"pc.safedog.cn",
	"studycenter.vko.cn",
	"ct2.jcwcn.com",
	"streaming.centv.cn",
	"media.guoshi.com",
	"live.cnlive.com",
	"btssii-ifc.swok.cn",
	"fj.123yun.cn",
	"www.hao556.com",
	"speedup.swok.cn",
	"download.ruanjian.2345.com",
	"download.cloud.189.cn",
	"voice-cc.mchang.cn",
	"bangbang.58.com",
	"tingapi.ting.baidu.com",
	"api.dingdong.ganji.com",
	"baohe.qqbaobao.com",
	"ui.ptlogin2.qq.com",
	"video.appledaily.com.hk",
	"videoad.nextdigital.com.hk",
	"video.next.hk",
	"video.cdn.on.cc",
	"www.ismeye.com",
	"web.topboce.cn",
	"client.118100.cn",
	"admin.crazylaiwang.hortorgames.com:8890",
	"crazyguess.hortorgame.com",
	"app100645222.qzone.qzoneapp.com",
	"www.ezjoy.cn",
	"res.a.cifoo.com",
	"pdata.video.qiyi.com",
	"www3.dfxiu8.com:88",
	"vimg.dwstatic.com",
	"live.gslb.letv.com",
	"www.hycxsy.com",
	"www.glsc.com.cn",
	"www.weistock.com",
	"hq.htsc.com.cn",
	"download.windowsupdate.com",
	"cd.m70s.com",
	"phoneimage.uucall.com",
	"apiv2.vmovier.com",
	"7rflo2.com2.z0.glb.qiniucdn.com",
	"t.wifiwldh.com",
	"dxsearch.baidu.com",
	"video.kascend.com:80",
	"download.3gwawa.net",
	"cdn.bluestacks.com",
	"pczhibo.imgo.tv",
	"pccvideofast.imgo.tv",
	"qqlive.hdl.lxdns.com:80",
	"qqlive.dnion.com:1863",
	"wscdn.qianmo.com",
	"msnms.allyes.com.cn",
	"vweixinf.tc.qq.com",
	"m20.lunhui.game2.com.cn",
	"v4.100tv.com",
	"leboapi.baidu.com",
	"woa.sdo.com",
	"cdnringhlt.shoujiduoduo.com",
	"mobileapp.ke.qq.com",
	"music.163.com",
	"www.51kan.com.cn",
	"phone.xfplay.com",
	"push.webmail.mail.163.com",
	"client.diyring.cc",
	"video.raventech.cn",
	"huluemt.youyuan.com",
	"music.doreso.com",
	"tips.meitu.com",
	"fc.dongao.com",
	"media1.icourses.cn",
	"api.app.caoooo.cn",
	"hbfile.b0.upaiyun.com",
	"www.gangsh.com",
	"a.tutuim.com",
	"lp-qiniu.gaypark.cn",
	"android.lespark.us",
	"mauth.chinanetcenter.com",
	"stats1.jiuyan.info",
	"a.in66.com",
	"v.eol.cn",
	"video.qpic.cn",
	"admin.iquyou.net",
	"app-homeinns.b0.upaiyun.com",
	"mp3tuijian.9ku.com",
	"yx.antblue.com",
	"hall.imixiu.cn",
	"116.213.115.115",
	"cu.acgvideo.com",
	"www.51hyc.com",
	"api.haodou.com",
	"coupon.ggeye.com",
	"zone6165.huoying.qq.com",
	"fwss.xiu.youku.com",
	"mapi.4399api.net",
	"a.img4399.com",
	"api.51job.com",
	"m.rong360.com",
	"j5.dfcfw.com",
	"res.iwoapp.com",
	"m.360buyimg.com",
	"read.xiaoshuo1-sm.com",
	"s2.zimgs.cn",
	"mapi.dangdang.com",
	"live.a.302.yunfancdn.com",
	"c.kp747.com",
	"lxdqncdn.miaopai.com",
	"qiubai-video.qiushibaike.com",
	"so.ard.iyyin.com",
	"game.yymxwk.90tank.com",
	"lefeng-api.vip.com",
	"lraudio.qiniucdn.com",
	"picture.duokan.com",
	"cctvjilu.vtime.cntv.cloudcdn.net",
	"earn.bs2dl.yy.com",
	"live.panda.tv",
	"sdkapp.mobile.sina.cn",
	"note.youdao.com",
	"beacon.tingyun.com",
	"www.baixing.com",
	"api.zank.mobi",
	"tongji.xueba100.com",
	"t.chinavivaki.com",
	"www.leiphone.com",
	"fuwu.sogou.com",
	"api.agent.sogou.com:",
	"lxcdn.slooti.com",
	"fs.web.kugou.com",
	"i.mmcdn.cn",
	"t.taobao.com",
	"wslive-hdl.kascend.com",
	"news.push.126.net",
	"g1.163.com",
	"img1.126.net",
	"api-app.meizu.com",
	"jijin.yundzh.com",
	"v2.yundzh.com:81",
	"mnews.gw.com.cn",
	"download.m.jj.cn",
	"appdata.pingan.com.cn",
	"icoreaopsactive.pingan.com.cn:",
	"static.imxingzhe.com",
	"uplus-banner.b0.upaiyun.com",
	"ptw.youyuan.com",
	"112.126.83.1:8081",
	"pptv.m.cn.miaozhen.com",
	"task.smartjump.pptv.com",
	"q.qlogo.cn",
	"211.151.99.70:7070",
	"www.fengqu.com",
	"m8.play.vp.autohome.com.cn",
	"sp.kaola.com",
	"mob.jebe.renren.com",
	"api.newtuding.zinantech.cn",
	"mb.51buy.com",
	"mobds.ganji.com",
	"stat.onbilin.com",
	"bbmapi.brtn.cn",
	"mmapp2.youyuan.com",
	"wsyyst.yy.com",
	"p.kuaidi100.com",
	"news.cfkd.net",
	"www.chinancce.com",
	"patch.towerofsaviors.com",
	"mall.gtja.com",
	"zxfile.tdx.com.cn",
	"www.chinastock.com.cn",
	"api.hello.yy.com",
	"imgsize.ph.126.net",
	"api.huaban.com",
	"121.43.67.164",
	"120.55.198.234:6666",
	"config.mobile.meituan.com",
	"mobileif.maizuo.com",
	"s.baixing.net",
	"dcps.nuomi.com",
	"v.navi.baidu.com",
	"www.baidu.com",
	"offnavi.map.baidu.com",
	"wappass.bdimg.com",
	"ulog.imap.baidu.com",
	"dl.imap.baidu.com",
	"route.navi.baidu.com",
	"api.11fen.cn",
	"q.11fen.cn",
	"i.11fen.cn",
	"s.same.com",
	"115.29.5.49",
	"guimi.qiniucdn.com",
	"api.jiemoapp.com",
	"p.jiemosrc.com",
	"static.tieba.baidu.com",
	"api.share.mob.com",
	"android.bugly.qq.com",
	"img.xiaohongshu.com",
	"www.taobao.com",
	"i.mchang.cn:80",
	"m.v5music.com",
	"s.moguupd.com",
	"lolbox.duowan.com",
	"funbox.duowan.com",
	"www.chaohuida.com",
	"v.gdt.qq.com",
	"img.bugu.18183.com",
	"log.snssdk.com",
	"mon.snssdk.com",
	"isub.snssdk.com",
	"ic.snssdk.com",
	"lf.snssdk.com",
	"ichannel.snssdk.com",
	"ib.snssdk.com",
	"track.appsflyer.com",
	"api.likeorz.com",
	"stats.jpush.cn",
	"cdn.likeorz.com",
	"conf.api.xiaoenai.com",
	"forum2.api.xiaoenai.com",
	"apps.api.xiaoenai.com",
	"noti.api.xiaoenai.com",
	"service5.ingkee.com",
	"7xjnx1.com2.z0.glb.qiniucdn.com",
	"qn-cover.zaizhibo.tv",
	"mobileapp.banggo.com:8081",
	"qjapp4.23yly.com",
	"621.api.aiba.com",
	"cp.qpic.cn",
	"app27036.static.kingnet.com",
	"i.wework.qq.com",
	"www.9158.com",
	"tyst.migu.cn",
	"upload.69xiu.com",
	"www.dianping.com",
	"awp-assets.meituan.net",
	"114.80.165.63",
	"m.yuntonghua.net",
	"apiserver.cctvweishi.com",
	"oss.cctvweishi.com",
	"mmmono.qiniudn.com",
	"api.xunaiba.cn",
	"630.api.renrenaijiaoyou.com",
	"42.96.251.40:80",
	"ws.stream.kg.qq.com",
	"fdfs.xmcdn.com",
	"c.hiphotos.baidu.com",
	"et.momocdn.com",
	"w.tancdn.com",
	"u.tancdn.com",
	"cloud.tantanapp.com",
	"api.youyuan8.cn",
	"tj-btfs.yun.ftn.qq.com",
	"default.ws.youja.cn:8080",
	"api.seasonmall.cn",
	"api.miaijiaoyou.cn",
	"203.195.213.243:9898",
	"api.xunlove.cn",
	"api.quanqiuyue.com",
	"kkhulu.youyuan.com",
	"khbapi.ciwong.com",
	"www.o3008o.com",
	"live.9158.com",
	"newapi.loukou.com",
	"api.meituan.com",
	"p0.meituan.net",
	"sapi.beibei.com",
	"d28julafmv4ekl.cloudfront.net",
	"dl.118100.cn:9495",
	"video2.peiyinxiu.com",
	"data.5sing.kgimg.com",
	"mobileapi.5sing.kugou.com",
	"goapi.5sing.kgimg.com",
	"5singlive.kugou.com",
	"nksingserver.kuwo.cn",
	"media.cdn.kuwo.cn",
	"d.ifengimg.com",
	"cxcnd.kuwo.cn",
	"tingshu.kuwo.cn",
	"share.m.kakamobi.com",
	"report.share.kakamobi.cn",
	"cheyouquan.image.mucang.cn",
	"120.24.67.13:8003",
	"182.92.244.70",
	"112.124.39.61",
	"120.55.151.61",
	"api.d3.com.cn:80",
	"icon.fenfenriji.com",
	"d.fenfenriji.com",
	"www.hdb.com",
	"api.shell001.com",
	"mapp.quxiu8.com",
	"lz.rili.cn",
	"mp4.res.hunantv.com",
	"mengine.go2map.com",
	"bookstore2.shuqireader.com",
	"img1.rrcimg.com",
	"qie-zi-pic.oss-cn-hangzhou.aliyuncs.com",
	"api.impingo.me",
	"wx.impingo.me",
	"a.impingo.me",
	"vhotlx.video.qq.com",
	"video.dispatch.tc.qq.com",
	"tj-btfs.yunup.ftn.qq.com",
	"api.xiaokaxiu.com",
	"vod.cntv.lxdns.com",
	"appapi.huipinzhe.com",
	"m.huipinzhe.com",
	"uploadv2.zhuanmi.net:443",
	"www.coolyun.com",
	"gz189cloud.oos-gz.ctyunapi.cn",
	"flv4mp4.people.com.cn",
	"www.smg.cn",
	"cp12.ccp2.play.bokecc.com",
	"vod.weathertv.cn",
	"img.ddsy.com",
	"o2o.dailyyoga.com.cn",
	"fq1.img.meidaila.net",
	"m.mdl.com",
	"www.qingniancaijun.com.cn",
	"gdc.hupucdn.com",
	"rom2.xianshuabao.com",
	"alicdnp.vod.max.mgtv.com",
	"map.baidu.com",
	"jipiao.trip.163.com",
	"pimg1.126.net",
	"flvtx.plu.cn",
	"api.letvcloud.com",
	"tpic.home.news.cn",
	"fms.news.cn",
	"www.1ting.com",
	"shop.cloudsee.com",
	"bbs.cloudsee.com",
	"live.cloudsee.com",
	"lvyou.baidu.com",
	"stats.g.doubleclick.net",
	"monitor.m.koudai.com",
	"api.public.koudai.com",
	"cdn.redfoundry.com",
	"i2.j.cn",
	"apic.x.yiche.com",
	"www.ycmm.com",
	"appxml.jyw8.com",
	"dynamic.12306.cn",
	"ws.tsmusic.kg.qq.com",
	"api.dongting.com",
	"3p.pic.ttdtweb.com",
	"webresource.c-ctrip.com",
	"live.xmcdn.com",
	"dimg04.c-ctrip.com",
	"kascdn.kascend.com",
	"longzhu.kssws.ks-cdn.com",
	"configapi.plu.cn",
	"z.api.lesports.com",
	"static.api.sports.letv.com",
	"sp.hls.yinyuetai.com",
	"119.188.180.103",
	"43.240.58.27:8080",
	"uri.52itv.cn",
	"xylive3.tvesou.com",
	"hls4.l.cztv.com",
	"pull.kktv8.com",
	"flv.quanmin.tv",
	"v.9eee.cn",
	"tc-tct.douyucdn.cn",
	"cdn.a9377j.com",
	"www.iavcusa.com",
	"www.icntv.net",
	"pl8-hdl.live.5kong.tv",
	"cietv.com",
	"djwy.cdn.gop.yy.com",
	"r.mapbar.com",
	"sdksso.map.qq.com",
	"v.ifeng.com",
	"img.mobile.kuxun.cn",
	"c2.taxifast.com.cn:80",
	"static.guojiang.tv",
	"rtmppull.efeizao.com",
	"hdl.v.ihani.tv",
	"momo.hdllive.ks-cdn.com",
	"image.xingzhe01.com",
	"dianbo.dn.xingzhe01.com",
	"api.cuctv.com",
	"mv03.cuctv.com",
	"api.xiaozhutv.com:10000",
	"img.znds.com",
	"imgsou.dangbei.com",
	"appsou.tvapk.com",
	"imgs.aixifan.com",
	"player.youku.com",
	"app.vmoiver.com",
	"pudding.cc",
	"c1.hoopchina.com.cn",
	"cc.dace.hupu.com",
	"video2.mukewang.com",
	"ac.qq.com",
	"logs.live.tudou.com",
	"ios.yingshi.1kxun.mobi",
	"api.helper.qq.com",
	"speed.coral.qq.com",
	"app.guojiang.tv",
	"121.41.53.245:81",
	"live33979224.w.api.zego.im",
	"androidapi.lansha.tv",
	"cncreplay.miguvideo.com",
	"drivers8_2016.updrv.com",
	"res.jzwl.52xiyou.com",
	"dt.2011web.com",
	"api.imheixiu.com",
	"m0.ifeng.com",
	"api.k.sohu.com",
	"api.my.tv.sohu.com",
	"s.cimg.163.com",
	"cnlive.bn.netease.com",
	"wbapp.mobile.sina.cn",
	"api.jk.cn",
	"ksinglive.kuwo.cn",
	"common.auto98.com",
	"calendar.lenovomm.com",
	"v2.kuaigeng.com",
	"init.startappexchange.com",
	"req.startappservice.com",
	"www.simplecreator.net",
	"47.88.34.76",
	"m.cn.happyelements.com",
	"888.qq.com",
	"v2.iqianjin.com",
	"www.gupiaogaoshou.com",
	"msgcenter.wacai.com",
	"platform.sina.com.cn",
	"www.youdoapp.net",
	"182.92.159.193:80",
	"api-c.yunpan.360.cn",
	"api.photo.yunpan.360.cn",
	"dnf.duowan.com",
	"szhuodong.duowan.com",
	"g.duowan.com",
	"down.tcjdbbx.com",
	"jt.soft.jdbbx.com",
	"api.edaijia.cn",
	"wot.duowan.com",
	"1-api-red.xunlei.com",
	"red.xunlei.com",
	"game.gtimg.cn",
	"lol.qq.com",
	"ossweb-img.qq.com",
	"qt.qq.com",
	"118.26.205.155",
	"meibo.hdllive.ks-cdn.com",
	"pili-live-hdl.videer.net",
	"image.vlook.cn:80",
	"download1.vlook.cn:80",
	"tv.aiseet.atianqi.com",
	"omgmta.play.aiseet.atianqi.com:80",
	"vbigts.tc.qq.com",
	"pic.tyread.com:8082",
	"client.tyread.com:8080",
	"app.perfectcorp.cn",
	"app-master.perfectcorp.cn",
	"cdn.beautycircle.com",
	"client2.310win.com",
	"client.fcaimao.com",
	"vip.dingtalk.com",
	"easyread.ph.126.net",
	"easyread.163.com",
	"chinanet3.vod.bestvcdn.com.cn",
	"bst.mobile.vod.bestvcdn.com.cn",
	"chinanet3.live.bestvcdn.com.cn",
	"mimgs.bestv.cn",
	"img.8zyl.cn",
	"pics.lehaitv.com",
	"service.rgbvr.com",
	"image.qn.rgbvr.com",
	"live2vod.kssws.ks-cdn.com",
	"ws.rgbvr.com:18282",
	"img.gametaptap.com",
	"d.taptapdada.com",
	"ksdownhdl.jufan.tv",
	"videodev.ksyun.com:8980",
	"media.999d.com",
	"waqu.com",
	"down.jia.360.cn",
	"hzp.yoka.com",
	"fujun.yoka.com",
	"boss.beautytreasure.cn",
	"boss.img.beautytreasure.cn",
	"soft.mhmp.cn",
	"c6.mhmp.cn",
	"123.59.24.12",
	"api.data.beautyplus.com",
	"www.thestylewish.com",
	"appwk.baidu.com",
	"bosstatic.wenku.bdimg.com",
	"img.baidu.com",
	"pic.189read.com",
	"src.kuaipai.cn",
	"app.www.gov.cn",
	"dl.cm.ksmobile.com",
	"image.mengdian.com",
	"missfresh-asvip-production-common.ufile.ucloud.com.cn",
	"c.migudm.cn",
	"119.29.147.108:8001",
	"123.58.165.171:7010",
	"fuwu.mojing.cn",
	"static.quhepai.com",
	"record-hls.server.cctalk.com",
	"www.docin.com",
	"img.laoyuegou.com",
	"appv2.lygou.cc",
	"adv.91xuexibao.com:3010",
	"res.o2o.com.cn:8082",
	"wkfind.o2o.com.cn:8080",
	"pull.ws.yazhai.com",
	"new.api.yazhai.com",
	"down.yazhai.com",
	"img.21sq.org",
	"api.21jingji.com",
	"media.winbaoxian.com",
	"img.winbaoxian.com",
	"service.winbaoxian.com",
	"9314.vod.myqcloud.com",
	"www.tvcbook.com",
	"www.finger66.com",

};


#define ARRAY_SIZE(array)	(sizeof(array)/sizeof(array[0]))

#define PERF_TEST_STR_CNT	(1024)
#define PERF_TEST_LOOPS		(100000)

struct str_set {
	char **str;
	int cnt;
};

struct str_node {
	struct str_node *next;
	char *str;
};

struct hash_set {
	struct str_node **nodes;
	int cnt;
};

struct perf_result {
	int match_times;
	int cost_secs;
};

typedef unsigned int u32;
typedef unsigned char u8;

typedef int bool;
#define true 1
#define false 0


#define JHASH_INITVAL           0xdeadbeef

struct __una_u32 { u32 x; } __packed;

static inline u32 __get_unaligned_cpu32(const void *p)
{
        const struct __una_u32 *ptr = (const struct __una_u32 *)p;
        return ptr->x;
}




static inline u32 rol32(u32 word, unsigned int shift)
{
        return (word << shift) | (word >> (32 - shift));
}

/* __jhash_mix -- mix 3 32-bit values reversibly. */
#define __jhash_mix(a, b, c)                    \
{                                               \
        a -= c;  a ^= rol32(c, 4);  c += b;     \
        b -= a;  b ^= rol32(a, 6);  a += c;     \
        c -= b;  c ^= rol32(b, 8);  b += a;     \
        a -= c;  a ^= rol32(c, 16); c += b;     \
        b -= a;  b ^= rol32(a, 19); a += c;     \
        c -= b;  c ^= rol32(b, 4);  b += a;     \
}

/* __jhash_final - final mixing of 3 32-bit values (a,b,c) into c */
#define __jhash_final(a, b, c)                  \
{                                               \
        c ^= b; c -= rol32(b, 14);              \
        a ^= c; a -= rol32(c, 11);              \
        b ^= a; b -= rol32(a, 25);              \
        c ^= b; c -= rol32(b, 16);              \
        a ^= c; a -= rol32(c, 4);               \
        b ^= a; b -= rol32(a, 14);              \
        c ^= b; c -= rol32(b, 24);              \
}

/***************************************************************************************************************/
static void performance_tests_with_iter_memcmp(int str_len, int str_cnt, int test_str_cnt, int loops);
static void performance_tests_with_hash_memcmp(int str_len, int str_cnt, int test_str_cnt, int loops);
static void performance_tests_hashmemcmp_specific_strs(int test_str_cnt, int loops);

static void generate_random_str(char *buf, int len);
static void create_random_str_set(struct str_set *str_set, int str_len, int str_cnt, struct exact_trie *trie);
static void destroy_random_str_set(struct str_set *str_set);
static void create_hash_set(struct hash_set *hash_set, struct str_set *str_set, int str_len);
static bool search_hash_set(struct hash_set *hash_set, char *str, int str_len);
static void destroy_hash_set(struct hash_set *hash_set);
static bool search_hash_set(struct hash_set *hash_set, char *str, int str_len);
static int cal_bucket_cnt(int total_node_cnt);
static int cal_bucket_index(char *data, u32 len, u32 bucket_cnt);
static u32 jhash(const void *key, u32 length, u32 initval);
static void create_specific_pattern_set(struct str_set *str_set, struct exact_trie *trie);
static void create_hash_set_with_strs(struct hash_set *hash_set, struct str_set *str_set);
static void create_specific_random_match_set(struct str_set *str_set, int str_cnt);
/***************************************************************************************************************/
int main(void)
{
	struct exact_trie *trie;
	struct exact_match match;
	int ret, i;

	trie = exact_trie_create();
	if (!trie) {
		fprintf(stderr, "Fail to create trie\n");
		exit(1);
	}

	for (i = 0; i < ARRAY_SIZE(pattern); ++i) {
		ret = exact_trie_add(trie, pattern[i], strlen(pattern[i]), (void*)(long)(i+1), NULL);
		if (ret != TRIE_STATUS_OK) {
			fprintf(stderr, "TestCase1: unexpected ret(%d) of string(%s)\n", 
				ret, pattern[i]);
			exit(1);
		}
	}
	fprintf(stdout, "TestCase1: Pass inserting multiple patterns\n");

	for (i = 0; i < ARRAY_SIZE(pattern); ++i) {
		ret = exact_trie_add(trie, pattern[i], strlen(pattern[i]), (void*)(long)i, NULL);
		if (ret != TRIE_STATUS_DUP_STR) {
			fprintf(stderr, "TestCase2: unexpected ret(%d) of string(%s)\n", 
				ret, pattern[i]);
			exit(1);
		}
	}
	fprintf(stdout, "TestCase2: Pass forbid duplicated patterns test\n");

	for (i = 0; i < ARRAY_SIZE(part_pattern); ++i) {
		ret = exact_trie_add(trie, part_pattern[i], strlen(part_pattern[i]), (void*)(long)(100+i+1), NULL);
		if (ret != TRIE_STATUS_OK) {
			fprintf(stderr, "TestCase3: unexpected ret(%d) of string(%s)\n", 
				ret, part_pattern[i]);
			exit(1);
		}
	}
	fprintf(stdout, "TestCase3: Pass inserting part patterns\n");

	exact_trie_finalize(trie);

	exact_trie_dump(trie);

	memset(&match, 0, sizeof(match));
	match.match_mode = TRIE_MODE_EXACT_MATCH;

	for (i = 0; i < ARRAY_SIZE(match_str); ++i) {
		ret = exact_trie_search(trie, match_str[i], strlen(match_str[i]), &match);
		if (ret != TRIE_STATUS_OK) {
			fprintf(stderr, "TestCase4: unexpected ret(%d) of string(%s)\n", 
				ret, match_str[i]);
			exit(1);
		}
		fprintf(stdout, "\tThe exact match result of \"%s\" is ", match_str[i]);
		exact_trie_match_show(&match);
	}
	fprintf(stdout, "TestCase4: Pass match pattern\n");

	for (i = 0; i < ARRAY_SIZE(no_match_str); ++i) {
		ret = exact_trie_search(trie, no_match_str[i], strlen(no_match_str[i]), &match);
		if (ret != TRIE_STATUS_NO_EXIST) {
			fprintf(stderr, "TestCase5: unexpected ret(%d) of string(%s)\n", 
				ret, no_match_str[i]);
			exit(1);
		}
	}
	fprintf(stdout, "TestCase5: Pass no_match pattern\n");

	match.match_mode = TRIE_MODE_PREFIX_MATCH;
	for (i = 0; i < ARRAY_SIZE(prefix_match_str); ++i) {
		ret = exact_trie_search(trie, prefix_match_str[i], strlen(prefix_match_str[i]), &match);
		if (ret != TRIE_STATUS_OK) {
			fprintf(stderr, "TestCases6: unexpectd ret(%d) of string(%s)\n",
				ret, prefix_match_str[i]);
			exit(1);
		}		
		fprintf(stdout, "\tThe prefix match result of \"%s\" is ", prefix_match_str[i]);
		exact_trie_match_show(&match);
	}
	fprintf(stdout, "TestCase6: Pass prefix_match\n");

	for (i = 0; i < ARRAY_SIZE(prefix_no_match_str); ++i) {
		ret = exact_trie_search(trie, prefix_no_match_str[i], strlen(prefix_no_match_str[i]), &match);
		if (ret != TRIE_STATUS_NO_EXIST) {
			fprintf(stderr, "TestCases7: unexpected ret(%d) of string(%s)\n",
				ret, prefix_no_match_str[i]);
			exit(1);
		}
	}
	fprintf(stdout, "TestCase7: Pass prefix_no_match\n");

	int match_times = 0;
	memset(&match, 0, sizeof(match));
	match.match_mode = TRIE_MODE_PREFIX_MATCH;
	match.cont_match = 1;
	while ((ret = exact_trie_search(trie, cont_match_str, strlen(cont_match_str), &match)) == TRIE_STATUS_OK) {
		match_times++;
		fprintf(stdout, "The prefix match result of \"%s\" is ", cont_match_str);
		exact_trie_match_show(&match);
	}
	if (match_times != cont_match_times) {
		fprintf(stderr, "TestCase8: Continue match only matches %u time, epxect %u\n",
			match_times, cont_match_times);
		exit(1);
	}
	fprintf(stdout, "TestCase8: Pass the cont_match\n");

	exact_trie_destroy(trie);


	fprintf(stdout, "\n\n\nPassed all test cases!!!\n\n\n");

	performance_tests_hashmemcmp_specific_strs(PERF_TEST_STR_CNT, PERF_TEST_LOOPS);

	performance_tests_with_iter_memcmp(4, 64, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);

/*	
	performance_tests_with_iter_memcmp(2, 8, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);
	performance_tests_with_iter_memcmp(2, 16, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);
	performance_tests_with_iter_memcmp(2, 32, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);
	
	performance_tests_with_iter_memcmp(4, 8, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);
	performance_tests_with_iter_memcmp(4, 16, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);
	performance_tests_with_iter_memcmp(4, 32, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);
	performance_tests_with_iter_memcmp(4, 64, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);
	performance_tests_with_iter_memcmp(4, 128, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);

	performance_tests_with_iter_memcmp(8, 8, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);
	performance_tests_with_iter_memcmp(8, 16, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);
	performance_tests_with_iter_memcmp(8, 32, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);
	performance_tests_with_iter_memcmp(8, 64, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);
	performance_tests_with_iter_memcmp(8, 128, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);
*/
	
	performance_tests_with_hash_memcmp(2, 64, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);
/*
	performance_tests_with_hash_memcmp(2, 128, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);
	performance_tests_with_hash_memcmp(2, 256, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);
	performance_tests_with_hash_memcmp(2, 512, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);
	
	performance_tests_with_hash_memcmp(4, 64, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);
	performance_tests_with_hash_memcmp(4, 128, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);
	performance_tests_with_hash_memcmp(4, 256, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);
	performance_tests_with_hash_memcmp(4, 512, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);
	
	performance_tests_with_hash_memcmp(8, 64, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);
	performance_tests_with_hash_memcmp(8, 128, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);
	performance_tests_with_hash_memcmp(8, 256, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);
	performance_tests_with_hash_memcmp(8, 512, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);
	
	performance_tests_with_hash_memcmp(16, 64, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);
	performance_tests_with_hash_memcmp(16, 128, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);
	performance_tests_with_hash_memcmp(16, 256, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);
	performance_tests_with_hash_memcmp(16, 512, PERF_TEST_STR_CNT, PERF_TEST_LOOPS);
*/
	return 0;
}

static void performance_tests_with_iter_memcmp(int str_len, int str_cnt, int test_str_cnt, int loops)
{
	struct exact_trie *trie;
	struct str_set pattern_set;
	struct str_set rand_str_set;
	struct perf_result mem_pf, trie_pf;
	int i, j, k;
	time_t start, end;

	memset(&pattern_set, 0, sizeof(pattern_set));

	trie = exact_trie_create();
	if (!trie) {
		fprintf(stderr, "Fail to create exact trie\n");
		exit(1);
	}

	create_random_str_set(&pattern_set, str_len, str_cnt, trie);	
	exact_trie_finalize(trie);

	create_random_str_set(&rand_str_set, str_len, test_str_cnt, NULL);

	memset(&mem_pf, 0, sizeof(mem_pf));
	memset(&trie_pf, 0, sizeof(trie_pf));

	start = time(NULL);
	for (i = 0; i < test_str_cnt; ++i) {
		for (j = 0; j < loops; ++j) {
			for (k = 0; k < str_cnt; ++k) {
				if (0 == memcmp(pattern_set.str[k], rand_str_set.str[i], str_len)) {
					mem_pf.match_times++;
					break;
				}
			}
		}
	}
	end = time(NULL);
	mem_pf.cost_secs = end-start;

	start = time(NULL);
	for (i = 0; i < test_str_cnt; ++i) {
		for (j = 0; j < loops; ++j) {
			struct exact_match match;

			memset(&match, 0, sizeof(match));
			if (TRIE_STATUS_OK == exact_trie_search(trie, rand_str_set.str[i], str_len, &match)) {
				trie_pf.match_times++;
			}
		}
	}
	end = time(NULL);
	trie_pf.cost_secs = end-start;

	destroy_random_str_set(&pattern_set);
	destroy_random_str_set(&rand_str_set);

	fprintf(stdout, "\n");
	fprintf(stdout, "Perf Test: ExactTrie with IterMemcmp\n");
	fprintf(stdout, "Insert %u random strings(length is %u):\n", str_cnt, str_len);
	fprintf(stdout, "Lookup %u random strings(loop %u times):\n", test_str_cnt, loops);
	fprintf(stdout, "IterMemcmp match %u times, cost %d secs\n", mem_pf.match_times, mem_pf.cost_secs);
	fprintf(stdout, "ExactTrie match %u times, cost %d secs\n", trie_pf.match_times, trie_pf.cost_secs);
	fprintf(stdout, "\n");

	exact_trie_destroy(trie);
}

static void performance_tests_with_hash_memcmp(int str_len, int str_cnt, int test_str_cnt, int loops)
{
	struct exact_trie *trie;
	struct hash_set hash_set;
	struct str_set pattern_set;
	struct str_set rand_str_set;
	struct perf_result hash_pf, trie_pf;
	int i, j;
	time_t start, end;

	memset(&pattern_set, 0, sizeof(pattern_set));

	trie = exact_trie_create();
	if (!trie) {
		fprintf(stderr, "Fail to create exact trie\n");
		exit(1);
	}

	create_random_str_set(&pattern_set, str_len, str_cnt, trie);	
	exact_trie_finalize(trie);

	create_hash_set(&hash_set, &pattern_set, str_len);

	create_random_str_set(&rand_str_set, str_len, test_str_cnt, NULL);

	memset(&hash_pf, 0, sizeof(hash_pf));
	memset(&trie_pf, 0, sizeof(trie_pf));

	start = time(NULL);
	for (i = 0; i < test_str_cnt; ++i) {
		for (j = 0; j < loops; ++j) {
			if (search_hash_set(&hash_set, rand_str_set.str[i], str_len)) {
				hash_pf.match_times++;
			}
		}
	}
	end = time(NULL);
	hash_pf.cost_secs = end-start;

	start = time(NULL);
	for (i = 0; i < test_str_cnt; ++i) {
		for (j = 0; j < loops; ++j) {
			struct exact_match match;

			memset(&match, 0, sizeof(match));
			if (TRIE_STATUS_OK == exact_trie_search(trie, rand_str_set.str[i], str_len, &match)) {
				trie_pf.match_times++;
			}
		}
	}
	end = time(NULL);
	trie_pf.cost_secs = end-start;

	destroy_random_str_set(&pattern_set);
	destroy_random_str_set(&rand_str_set);

	fprintf(stdout, "\n");
	fprintf(stdout, "Perf Test: ExactTrie with HashMemcmp\n");
	fprintf(stdout, "Insert %u random strings(length is %u):\n", str_cnt, str_len);
	fprintf(stdout, "Lookup %u random strings(loop %u times):\n", test_str_cnt, loops);
	fprintf(stdout, "HashMemcmp match %u times, cost %d secs\n", hash_pf.match_times, hash_pf.cost_secs);
	fprintf(stdout, "ExactTrie match %u times, cost %d secs\n", trie_pf.match_times, trie_pf.cost_secs);
	fprintf(stdout, "\n");

	exact_trie_destroy(trie);
	destroy_hash_set(&hash_set);
}

static void performance_tests_hashmemcmp_specific_strs(int test_str_cnt, int loops)
{	
	struct exact_trie *trie;
	struct hash_set hash_set;
	struct str_set pattern_set;
	struct str_set rand_str_set;
	struct str_set rand_match_str_set;
	struct perf_result hash_pf, trie_pf;
	int i, j;
	time_t start, end;

	memset(&pattern_set, 0, sizeof(pattern_set));

	trie = exact_trie_create();
	if (!trie) {
		fprintf(stderr, "Fail to create exact trie\n");
		exit(1);
	}

	create_specific_pattern_set(&pattern_set, trie);
	create_hash_set_with_strs(&hash_set, &pattern_set);
	exact_trie_finalize(trie);

	/* Random str test */	
	create_random_str_set(&rand_str_set, 10, test_str_cnt, NULL);

	memset(&hash_pf, 0, sizeof(hash_pf));
	memset(&trie_pf, 0, sizeof(trie_pf));

	start = time(NULL);
	for (i = 0; i < test_str_cnt; ++i) {
		for (j = 0; j < loops; ++j) {
			if (search_hash_set(&hash_set, rand_str_set.str[i], 10)) {
				hash_pf.match_times++;
			}
		}
	}
	end = time(NULL);
	hash_pf.cost_secs = end-start;

	start = time(NULL);
	for (i = 0; i < test_str_cnt; ++i) {
		for (j = 0; j < loops; ++j) {
			struct exact_match match;

			memset(&match, 0, sizeof(match));
			if (TRIE_STATUS_OK == exact_trie_search(trie, rand_str_set.str[i], 10, &match)) {
				trie_pf.match_times++;
			}
		}
	}
	end = time(NULL);
	trie_pf.cost_secs = end-start;
	destroy_random_str_set(&rand_str_set);

	fprintf(stdout, "\n");
	fprintf(stdout, "Perf Test: Specific Strings\n");
	fprintf(stdout, "Insert %lu specific strings:\n", ARRAY_SIZE(g_exact_strs));
	fprintf(stdout, "Lookup %u random strings(loop %u times):\n", test_str_cnt, loops);
	fprintf(stdout, "HashMemcmp match %u times, cost %d secs\n", hash_pf.match_times, hash_pf.cost_secs);
	fprintf(stdout, "ExactTrie match %u times, cost %d secs\n", trie_pf.match_times, trie_pf.cost_secs);
	fprintf(stdout, "\n");


	create_specific_random_match_set(&rand_match_str_set, test_str_cnt);
	
	memset(&hash_pf, 0, sizeof(hash_pf));
	memset(&trie_pf, 0, sizeof(trie_pf));

	start = time(NULL);
	for (i = 0; i < test_str_cnt; ++i) {
		for (j = 0; j < loops; ++j) {
			if (search_hash_set(&hash_set, rand_match_str_set.str[i], strlen(rand_match_str_set.str[i]))) {
				hash_pf.match_times++;
			} else {
				printf("%s is not matched\n", rand_match_str_set.str[i]);
				exit(1);
			}
		}
	}
	end = time(NULL);
	hash_pf.cost_secs = end-start;

	start = time(NULL);
	for (i = 0; i < test_str_cnt; ++i) {
		for (j = 0; j < loops; ++j) {
			struct exact_match match;

			memset(&match, 0, sizeof(match));
			if (TRIE_STATUS_OK == exact_trie_search(trie, rand_match_str_set.str[i], strlen(rand_match_str_set.str[i]), &match)) {
				trie_pf.match_times++;
			}
		}
	}
	end = time(NULL);
	trie_pf.cost_secs = end-start;
	destroy_random_str_set(&rand_match_str_set);

	fprintf(stdout, "\n");
	fprintf(stdout, "Perf Test: Specific Strings\n");
	fprintf(stdout, "Insert %lu specific strings:\n", ARRAY_SIZE(g_exact_strs));
	fprintf(stdout, "Lookup %u random match strings(loop %u times):\n", test_str_cnt, loops);
	fprintf(stdout, "HashMemcmp match %u times, cost %d secs\n", hash_pf.match_times, hash_pf.cost_secs);
	fprintf(stdout, "ExactTrie match %u times, cost %d secs\n", trie_pf.match_times, trie_pf.cost_secs);
	fprintf(stdout, "\n");
	

	destroy_random_str_set(&pattern_set);
	exact_trie_destroy(trie);
	destroy_hash_set(&hash_set);
}


static int cal_bucket_cnt(int total_node_cnt)
{
	int cnt = total_node_cnt/4;

	if (cnt > 2048) {
		cnt = 2048;
	}

	return cnt;
}

static int cal_bucket_index(char *data, u32 len, u32 bucket_cnt)
{
	return jhash(data, len, 0x12345678)%bucket_cnt;
}


static void create_random_str_set(struct str_set *str_set, int str_len, int str_cnt, struct exact_trie *trie)
{
	int ret, i;
	
	memset(str_set, 0, sizeof(*str_set));

	str_set->str = malloc(sizeof(*str_set->str)*str_cnt);
	if (!str_set->str) {
		fprintf(stderr, "Fail to malloc\n");
		exit(1);
	}
	
	/* prepare the random pattern */
	for (i = 0; i < str_cnt; ++i) {
		str_set->str[i] = malloc(str_len);
		if (!str_set->str[i]) {
			fprintf(stderr, "Fail to malloc\n");
			exit(1);
		}

		if (trie) {
			do {
				generate_random_str(str_set->str[i], str_len);
				ret = exact_trie_add(trie, str_set->str[i], str_len, NULL, NULL);
			} while (ret != TRIE_STATUS_OK);
		}
	}
	str_set->cnt = str_cnt;
}

static void destroy_random_str_set(struct str_set *str_set)
{
	int i;

	for (i = 0; i < str_set->cnt; ++i) {
		free(str_set->str[i]);
	}

	free(str_set->str);
}

static void create_hash_set(struct hash_set *hash_set, struct str_set *str_set, int str_len)
{
	int i;

	memset(hash_set, 0, sizeof(*hash_set));
	hash_set->cnt = cal_bucket_cnt(str_set->cnt);
	hash_set->nodes = malloc(sizeof(*hash_set->nodes)*hash_set->cnt);
	memset(hash_set->nodes, 0, sizeof(*hash_set->nodes)*hash_set->cnt);

	for (i = 0; i < str_set->cnt; ++i) {
		int index = cal_bucket_index(str_set->str[i], str_len, hash_set->cnt);
		struct str_node *node = malloc(sizeof(*node));

		memset(node, 0, sizeof(*node));
		node->str = malloc(str_len);
		memcpy(node->str, str_set->str[i], str_len);
		node->next = NULL;

		if (hash_set->nodes[index]) {
			node->next = hash_set->nodes[index];
		} 
		hash_set->nodes[index] = node;
	}
}

static bool search_hash_set(struct hash_set *hash_set, char *str, int str_len)
{	
	int index = cal_bucket_index(str, str_len, hash_set->cnt);
	struct str_node *node = hash_set->nodes[index];
	
	while (node) {
		if (0 == memcmp(node->str, str, str_len)) {
			return true;
		}
		node = node->next;
	}

	return false;
}


static void destroy_hash_set(struct hash_set *hash_set)
{
	int i;

	for (i = 0; i < hash_set->cnt; ++i) {
		struct str_node *next;
		
		while (hash_set->nodes[i]) {
			next = hash_set->nodes[i]->next;
			free(hash_set->nodes[i]->str);
			free(hash_set->nodes[i]);
			hash_set->nodes[i] = next;
		}
	}

	free(hash_set->nodes);
}

static void generate_random_str(char *buf, int len)
{
	int fd;

	fd = open("/dev/urandom", O_RDONLY);

	read(fd, buf, len);

	close(fd);
}


static u32 jhash(const void *key, u32 length, u32 initval)
{
        u32 a, b, c;
        const u8 *k = key;

        /* Set up the internal state */
        a = b = c = JHASH_INITVAL + length + initval;

        /* All but the last block: affect some 32 bits of (a,b,c) */
        while (length > 12) {
                a += __get_unaligned_cpu32(k);
                b += __get_unaligned_cpu32(k + 4);
                c += __get_unaligned_cpu32(k + 8);
                __jhash_mix(a, b, c);
                length -= 12;
                k += 12;
        }
        /* Last block: affect all 32 bits of (c) */
        /* All the case statements fall through */
        switch (length) {
        case 12: c += (u32)k[11]<<24;
        case 11: c += (u32)k[10]<<16;
        case 10: c += (u32)k[9]<<8;
        case 9:  c += k[8];
        case 8:  b += (u32)k[7]<<24;
        case 7:  b += (u32)k[6]<<16;
        case 6:  b += (u32)k[5]<<8;
        case 5:  b += k[4];
        case 4:  a += (u32)k[3]<<24;
        case 3:  a += (u32)k[2]<<16;
        case 2:  a += (u32)k[1]<<8;
        case 1:  a += k[0];
                 __jhash_final(a, b, c);
        case 0: /* Nothing left to add */
                break;
        }

        return c;
}

static void create_specific_pattern_set(struct str_set *str_set, struct exact_trie *trie)
{
	int ret, i;
	int str_cnt = ARRAY_SIZE(g_exact_strs);
	
	memset(str_set, 0, sizeof(*str_set));

	str_set->str = malloc(sizeof(*str_set->str)*str_cnt);
	if (!str_set->str) {
		fprintf(stderr, "Fail to malloc\n");
		exit(1);
	}
	
	/* prepare the random pattern */
	for (i = 0; i < str_cnt; ++i) {
		int str_len = strlen(g_exact_strs[i]);
		str_set->str[i] = strdup(g_exact_strs[i]);
		if (trie) {			
			ret = exact_trie_add(trie, g_exact_strs[i], str_len, NULL, NULL);
			if (ret == TRIE_STATUS_DUP_STR) {
				fprintf(stderr, "%s is duplicated\n", g_exact_strs[i]);
				exit(1);
			}
		}
	}
	str_set->cnt = str_cnt;
}

static void create_specific_random_match_set(struct str_set *str_set, int str_cnt)
{
	int i;
	
	memset(str_set, 0, sizeof(*str_set));

	str_set->str = malloc(sizeof(*str_set->str)*str_cnt);
	if (!str_set->str) {
		fprintf(stderr, "Fail to malloc\n");
		exit(1);
	}

	srandom(0x12345678);
	
	/* prepare the random pattern */
	for (i = 0; i < str_cnt; ++i) {
		int index = random()%ARRAY_SIZE(g_exact_strs);	
		str_set->str[i] = strdup(g_exact_strs[index]);
	}
	str_set->cnt = str_cnt;
}


static void create_hash_set_with_strs(struct hash_set *hash_set, struct str_set *str_set)
{
	int i;

	memset(hash_set, 0, sizeof(*hash_set));
	hash_set->cnt = cal_bucket_cnt(str_set->cnt);
	hash_set->nodes = malloc(sizeof(*hash_set->nodes)*hash_set->cnt);
	memset(hash_set->nodes, 0, sizeof(*hash_set->nodes)*hash_set->cnt);

	for (i = 0; i < str_set->cnt; ++i) {
		int index = cal_bucket_index(str_set->str[i], strlen(str_set->str[i]), hash_set->cnt);
		struct str_node *node = malloc(sizeof(*node));

		memset(node, 0, sizeof(*node));
		node->str = strdup(str_set->str[i]);
		node->next = NULL;

		if (hash_set->nodes[index]) {
			node->next = hash_set->nodes[index];
		} 
		hash_set->nodes[index] = node;
	}
}


