# UP-Assistant

https://api.bilibili.com/x/web-interface/archive/stat?bvid=BV1Mq4y1g7wE

```json
{
  "code": 0,
  "message": "0",
  "ttl": 1,
  "data": {
    "aid": 549331981,
    "bvid": "BV1Mq4y1g7wE",
    "view": 1392844,
    "danmaku": 26844,
    "reply": 6669,
    "favorite": 20551,
    "coin": 31904,
    "share": 7359,
    "like": 134312,
    "now_rank": 0,
    "his_rank": 27,
    "no_reprint": 1,
    "copyright": 1,
    "argue_msg": "",
    "evaluation": ""
  }
}
```

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

获取cid: https://api.bilibili.com/x/player/pagelist?bvid=BV1Mq4y1g7wE&jsonp=jsonp

```json
{
  "code": 0,
  "message": "0",
  "ttl": 1,
  "data": [
    {
      "cid": 444923218,
      "page": 1,
      "from": "vupload",
      "part": "《原神》小剧场——「蒙德茶会」第四期",
      "duration": 2818,
      "vid": "",
      "weblink": "",
      "dimension": {
        "width": 1920,
        "height": 1080,
        "rotate": 0
      },
      "first_frame": "http://i1.hdslb.com/bfs/storyff/n211119a2isrra3cu85zl1yp8yvl07o4_firsti.jpg"
    }
  ]
}
```