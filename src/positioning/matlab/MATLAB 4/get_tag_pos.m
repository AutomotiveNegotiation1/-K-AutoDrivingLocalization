function tag_pos_est = get_tag_pos(tag_center_pos_est, heading_est, tag_pos_b)

tag_pos_est = tag_pos_b*exp(j*heading_est)+tag_center_pos_est;
