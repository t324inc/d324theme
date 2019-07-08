<?php

/**
 * Implements hook_preprocess_page().
 *
 */
function d324theme_preprocess_page(&$variables) {
  $variables['route_name'] = \Drupal::routeMatch()->getRouteName();
}