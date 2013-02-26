
var plugin = undefined;

function Plugin() {
    /** @private */
    this.container_ = document.getElementById("container");

    /** @private */
    this.screen_ = document.getElementById("screen");

    /** @private */
    this.button_ = document.getElementById("button");

    this.rotation_ = 0;
    document.body.addEventListener("click", function() {
        this.rotation_++;
    }.bind(this));

    this.lastFrame_ = 0;
    window.setInterval(function() {
      console.log(this.frame_ - this.lastFrame_);
      this.lastFrame_ = this.frame_;
    }.bind(this), 1000);

    this.test_(0);

    this.frame_ = 0;
}


Plugin.prototype.rander_ = function() {
    this.frame_++;
    this.changeRotation_(this.rotation_ % 4);
    if ((this.frame_ & 1) == 0)
      this.screen_.style.display = 'none';
    else
      this.screen_.style.display = 'block';
};

Plugin.prototype.test_ = function(i) {
  window.requestAnimationFrame(function() {
    this.rander_();
    this.test_();
  }.bind(this));
};

/** @private */
Plugin.prototype.changeRotation_ = function(rotation) {
    const PADDING = 10;
    var deg = rotation * 90;
    if (deg == 0)
      this.screen_.style['-webkit-transform'] = 'none';
    else
      this.screen_.style['-webkit-transform'] = 'rotate(-' + deg + 'deg)';
    var width, height, marginTop, marginLeft;
    if ((rotation & 1) == 1) {
        width = this.screen_.height;
        height = this.screen_.width;
        marginTop = PADDING - (width - height) / 2;
        marginLeft = PADDING - (height - width) / 2;
    } else {
        width = this.screen_.width;
        height = this.screen_.height;
        marginTop = PADDING;
        marginLeft = PADDING;
    }

    this.container_.style.top = '0px';
    this.container_.style.left = '0px';
    this.container_.style.width = (width + PADDING * 2) + 'px';
    this.container_.style.height = (height + 40  + PADDING * 2) + 'px';

    this.screen_.style['top'] = marginTop + 'px';
    this.screen_.style['left'] = marginLeft + 'px';

    this.button_.style['left'] = PADDING + 'px';
    this.button_.style['width'] = width + 'px';
    this.button_.style['top'] = (height + PADDING) + 'px';
};

window.onload=function() {
    if (plugin == undefined)
        plugin = new Plugin();
};
