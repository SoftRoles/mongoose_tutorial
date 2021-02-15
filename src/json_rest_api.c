// Copyright (c) 2015 Cesanta Software Limited
// All rights reserved

#include <stdlib.h>

#include "mongoose.h"

static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;

static void json_rest_api_handler(struct mg_connection *nc, int ev, void *ev_data)
{
  struct http_message *hm = (struct http_message *)ev_data;

  switch (ev)
  {
  case MG_EV_HTTP_REQUEST:
    printf("- HTTP Request received.\n");
    printf("-- HTTP Request received. Message: %.*s\n", hm->message.len, hm->message.p);
    printf("-- HTTP Request received. Proto: %.*s\n", hm->proto.len, hm->proto.p);
    printf("-- HTTP Request received. URL: %.*s\n", hm->uri.len, hm->uri.p);
    printf("-- HTTP Request received. Method: %.*s\n", hm->method.len, hm->method.p);
    printf("-- HTTP Request received. Query String: %.*s\n", hm->query_string.len, hm->query_string.p);
    char *query_str = malloc(hm->query_string.len + 1);
    // printf("%d\n",strlen(keyval));
    strncpy_s(query_str, hm->query_string.len + 1, hm->query_string.p, hm->query_string.len);
    query_str[hm->query_string.len] = '\0';
    printf("%s\n", query_str);

    char *keyval = strtok_s(query_str, hm->query_string.len + 1, "&");
    while (keyval != NULL)
    {
      printf("%s: %d\n", keyval, strlen(keyval)); //printing each token

      //   // // set_constraint_handler_s(abort_handler_s);
      //   // keyval = malloc(strlen(query_str_temp) + 1);
      //   // strncpy(keyval, query_str_temp, strlen(query_str_temp));
      //   // keyval[strlen(query_str_temp)] = '\0';
      //   // printf("%s\n", keyval);
      //   // char *keyval_tmp = strtok(keyval, "=");
      //   // printf("%s\n", keyval_tmp); //printing each token
      //   // keyval_tmp = strtok(NULL, "=");
      //   // printf("%s\n", keyval_tmp); //printing each token
      //   // free(keyval);

      keyval = strtok(NULL, "&");
      //   // printf("%s\n", query_str);
      //   // printf("%s\n", key); //printing each token
      //   // key = strtok(NULL, "=");
      //   // printf("%s\n", query_str);
      //   // printf("%s\n", key); //printing each token
    }
    free(query_str);
    printf("-- HTTP Request received. Body: %.*s\n", hm->body.len, hm->body.p);
    // printf("" & hm->uri->p);
    if (mg_vcmp(&hm->uri, "/api") == 0)
    {
      // handle_sum_call(nc, hm); /* Handle RESTful call */
    }
    else if (mg_vcmp(&hm->uri, "/printcontent") == 0)
    {
      char buf[100] = {0};
      memcpy(buf, hm->body.p,
             sizeof(buf) - 1 < hm->body.len ? sizeof(buf) - 1 : hm->body.len);
      printf("%s\n", buf);
    }
    else
    {
      mg_serve_http(nc, hm, s_http_server_opts); /* Serve static content */
    }
    break;

  default:
    break;
  }
}

int json_rest_api()
{
  struct mg_mgr mgr;
  struct mg_connection *nc;

  mg_mgr_init(&mgr, NULL);
  nc = mg_bind(&mgr, s_http_port, json_rest_api_handler);

  // Set up HTTP server parameters
  mg_set_protocol_http_websocket(nc);
  s_http_server_opts.document_root = ".";     // Serve current directory
  s_http_server_opts.dav_document_root = "."; // Allow access via WebDav
  s_http_server_opts.enable_directory_listing = "yes";

  printf("Starting web server on port %s\n", s_http_port);
  for (;;)
  {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

  return 0;
}