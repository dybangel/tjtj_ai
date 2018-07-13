// Action script...

// [Action in Frame 1]
totalB = this.getBytesTotal();
loadB = this.getBytesLoaded();
percent = int(loadB / totalB * 100);
bar.gotoAndStop(percent);
fscommand("fullscreen", "true");

// [Action in Frame 2]
if (percent < 100)
{
    gotoAndPlay(1);
}
else
{
    gotoAndPlay(menu);
} // end else if

// [Action in Frame 3]
stop ();
ss == 0;
backs = new Sound(_level1);
backs.attachSound("music1");
backs.start(0, 999);
mm == 1;
fscommand("fullscreen", "true");

// [Action in Frame 27]
if (ss == 1)
{
    gotoAndPlay(panpan);
} // end if
if (ss == 2)
{
    gotoAndPlay(birdie);
} // end if
if (ss == 3)
{
    gotoAndPlay(popo);
} // end if
if (ss == 4)
{
    gotoAndPlay(learn);
} // end if
if (ss == 5)
{
    gotoAndPlay(funny);
} // end if
backs = new Sound(_level1);
backs.attachSound("music2");
backs.start(0, 999);
mm == 2;

// [Action in Frame 46]
stop ();

// [Action in Frame 47]
stop ();

// [Action in Frame 66]
stop ();

// [Action in Frame 86]
stop ();

// [Action in Frame 105]
stop ();

// [Action in Frame 128]
stop ();
