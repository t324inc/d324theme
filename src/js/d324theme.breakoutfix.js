(function ($, Drupal) {

  'use strict';

  Drupal.behaviors.breakoutfix = {
    attach: function (context) {
      $('body', context).once('breakoutFixed').each(function() {
        $('body').addClass('breakout-fixed');
        handleWindow();
        $('window').on('resize', handleWindow);
      });

      function handleWindow() {
        var b = $('body');
        var normalw = window.innerWidth;
        var scrollw = normalw - b.width();
        var newStyle = '<style id="breakoutStyle">.breakout-container { left: calc(-50vw + 50% + (' + scrollw + 'px / 2)); width: calc(100vw - ' + scrollw + 'px); }</style>';
        var oldStyle = $("#breakoutStyle");
        if(oldStyle.length) {
          if(oldStyle[0].outerHTML != newStyle) {
            oldStyle.remove();
            $(newStyle).appendTo('head');
          }
        } else {
          $(newStyle).appendTo('head');
        }
      }
    }
  };



})(jQuery, Drupal);
