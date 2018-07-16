package com.baidu.android.voicedemo.activity;

import com.baidu.android.voicedemo.activity.setting.OnlineSetting;
import com.baidu.android.voicedemo.recognization.CommonRecogParams;
import com.baidu.android.voicedemo.recognization.online.OnlineRecogParams;

/**
 * 在线识别，用于展示在线情况下的识别参数和效果。
 * <p>
 * 本类可以忽略
 * ActivityRecog 识别流程，看下ActivityRecog开头的注释
 */
public class ActivityOnline extends ActivityRecog {
    {
        descText = "在线普通识别功能(含长语音)\n"
                + "请保持设备联网，对着麦克风说出日常用语即可\n"
                + "普通录音限制60s。使用长语音无此限制， 点击“设置”按钮，开启长语音选项后，请选择PID 1537。\n"
                + "\n"
                + "集成指南：\n"
                + "1. 关注下UI和Logcat中显示的识别参数 \n"
                + "2. ASR_START 不可连续调用，需要引擎空闲或者ASR_CANCEL输入事件后调用。详细请参见文档ASR_START的描述\n";
    }

    public ActivityOnline() {
        super();
        settingActivityClass = OnlineSetting.class;
    }

    @Override
    protected CommonRecogParams getApiParams() {
        return new OnlineRecogParams(this);
    }


}