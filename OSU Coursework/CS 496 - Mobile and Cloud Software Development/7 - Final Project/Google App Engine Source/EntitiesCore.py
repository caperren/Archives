from google.appengine.ext import ndb


class Project(ndb.Model):
    name = ndb.StringProperty(required=True)
    language = ndb.StringProperty()
    contributors = ndb.StringProperty(repeated=True)
    reminders = ndb.StringProperty(repeated=True)
    id = ndb.StringProperty()


class ProjectReminder(ndb.Model):
    title = ndb.StringProperty(required=True)
    reminder_date = ndb.StringProperty()
    users_to_notify = ndb.StringProperty(repeated=True)
    text_content = ndb.StringProperty()
    id = ndb.StringProperty()
