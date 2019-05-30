<?php
/**
 * @file
 * Theme and preprocess functions for nodes.
 */

function d324theme_preprocess_node__event(&$variables) {
  $event = $variables['node'];
  $date = $event->get('field_event_date')->getValue()[0]['value'];
  $date = strtotime($date);
  $date_month = date("M", $date);
  $date_day = date("d", $date);
  $variables['date_month'] = $date_month;
  $variables['date_day'] = $date_day;
}

function d324theme_preprocess_node__blog_post(&$variables) {
  $post = $variables['node'];
  $created = $post->get('created')->getValue()[0]['value'];
  $variables['post_date'] = date('M jS, Y', $created);
}
