<?php

function d324theme_preprocess_paragraph(&$variables) {
  $variables['background'] = FALSE;
  $paragraph = $variables['paragraph'];
  if($paragraph->hasField('field_background_image')) {
    $field = $paragraph->get('field_background_image')->first();
    if ($field) {
      $image_field = $paragraph->get('field_background_image')
        ->first()
        ->getValue();
      $media = \Drupal\media\Entity\Media::load($image_field['target_id']);
      $url = _d324_media_image_url($media, 'de2e');
      $variables['background'] = $url;
    }
  }
}
