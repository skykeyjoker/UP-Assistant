# UP-Assistant

获取视频信息：
https://api.bilibili.com/x/web-interface/view?bvid=BV1VL411M7N6

```json
{
  "code": 0,
  "message": "0",
  "ttl": 1,
  "data": {
    "bvid": "BV1VL411M7N6",
    "aid": 464385686,
    "videos": 1,
    "tid": 27,
    "tname": "综合",
    "copyright": 1,
    "pic": "http://i2.hdslb.com/bfs/archive/48acbd962378f782ad315c67b583ece5fa5ea80f.jpg",
    "title": "嘉然的全场最佳特写",
    "pubdate": 1637716518,
    "ctime": 1637672428,
    "desc": "然然来了全biu~了\n@A-SOUL_Official  \n嘉然@嘉然今天吃什么  \n向晚@向晚大魔王  \n乃琳@乃琳Queen  \n贝拉@贝拉kira",
    "desc_v2": [
      {
        "raw_text": "然然来了全biu~了\n",
        "type": 1,
        "biz_id": 0
      },
      {
        "raw_text": "A-SOUL_Official",
        "type": 2,
        "biz_id": 703007996
      },
      {
        "raw_text": " \n嘉然",
        "type": 1,
        "biz_id": 0
      },
      {
        "raw_text": "嘉然今天吃什么",
        "type": 2,
        "biz_id": 672328094
      },
      {
        "raw_text": " \n向晚",
        "type": 1,
        "biz_id": 0
      },
      {
        "raw_text": "向晚大魔王",
        "type": 2,
        "biz_id": 672346917
      },
      {
        "raw_text": " \n乃琳",
        "type": 1,
        "biz_id": 0
      },
      {
        "raw_text": "乃琳Queen",
        "type": 2,
        "biz_id": 672342685
      },
      {
        "raw_text": " \n贝拉",
        "type": 1,
        "biz_id": 0
      },
      {
        "raw_text": "贝拉kira",
        "type": 2,
        "biz_id": 672353429
      }
    ],
    "state": 0,
    "duration": 26,
    "rights": {
      "bp": 0,
      "elec": 0,
      "download": 1,
      "movie": 0,
      "pay": 0,
      "hd5": 1,
      "no_reprint": 1,
      "autoplay": 1,
      "ugc_pay": 0,
      "is_cooperation": 0,
      "ugc_pay_preview": 0,
      "no_background": 0,
      "clean_mode": 0,
      "is_stein_gate": 0,
      "is_360": 0,
      "no_share": 0
    },
    "owner": {
      "mid": 12585189,
      "name": "理塘J真",
      "face": "http://i0.hdslb.com/bfs/face/a4cee84f00da2daf59e5f9ebfda48d2520bc5cf3.jpg"
    },
    "stat": {
      "aid": 464385686,
      "view": 126301,
      "danmaku": 29,
      "reply": 280,
      "favorite": 1579,
      "coin": 527,
      "share": 1310,
      "now_rank": 0,
      "his_rank": 0,
      "like": 4649,
      "dislike": 0,
      "evaluation": "",
      "argue_msg": ""
    },
    "dynamic": "",
    "cid": 448174390,
    "dimension": {
      "width": 1920,
      "height": 1080,
      "rotate": 0
    },
    "no_cache": false,
    "pages": [
      {
        "cid": 448174390,
        "page": 1,
        "from": "vupload",
        "part": "成品_2",
        "duration": 26,
        "vid": "",
        "weblink": "",
        "dimension": {
          "width": 1920,
          "height": 1080,
          "rotate": 0
        },
        "first_frame": "http://i2.hdslb.com/bfs/storyff/n211124a2ia2gewyfgwo7p3dcy0kcq7s_firsti.jpg"
      }
    ],
    "subtitle": {
      "allow_submit": false,
      "list": []
    },
    "user_garb": {
      "url_image_ani_cut": "http://i0.hdslb.com/bfs/garb/item/f9a3f4aadb1cf268fc411c7b4cc99d07df3e778a.bin"
    },
    "honor_reply": {}
  }
}
```

获取正在观看人数：
https://api.bilibili.com/x/player/online/total?aid=549331981&cid=444923218&bvid=BV1Mq4y1g7wE

```json
{
  "code": 0,
  "message": "0",
  "ttl": 1,
  "data": {
    "total": "426",
    "count": "216",
    "show_switch": {
      "total": true,
      "count": true
    },
    "abtest": {
      "group": "b"
    }
  }
}
```
