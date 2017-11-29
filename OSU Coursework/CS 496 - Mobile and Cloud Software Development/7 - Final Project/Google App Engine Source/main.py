#!/usr/bin/env python

import webapp2

from RestAPIHandlersCore import RestProjectHandler, RestProjectReminderHandler

allowed_methods = webapp2.WSGIApplication.allowed_methods
new_allowed_methods = allowed_methods.union(('PATCH',))
webapp2.WSGIApplication.allowed_methods = new_allowed_methods

app = webapp2.WSGIApplication([
    webapp2.Route("/projects", RestProjectHandler, name="project-handler"),
    webapp2.Route("/projects/<project_id>", RestProjectHandler, name="project-handler"),
    webapp2.Route("/projects/<project_id>/reminders", RestProjectReminderHandler, name="reminder-handler"),
    webapp2.Route("/projects/<project_id>/reminders/<reminder_id>", RestProjectReminderHandler, name="reminder-handler")
], debug=True)
