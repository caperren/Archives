package com.example.corwinperren.cs496finalandroid;

public class APIUrls {

    public static String scheme = "https://";
    public static String host = "cs496-final-gae.appspot.com";
    public static String base_url = scheme + host;

    public static String get_projects_url = base_url + "/projects";
    public static String post_projects_url = get_projects_url;
}
