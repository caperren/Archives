import webapp2
import json
from google.appengine.ext import ndb

from EntitiesCore import Project, ProjectReminder


class RestProjectHandler(webapp2.RequestHandler):
    NEW_PROJECT_REQUIRED_POST_VALUES = ["name"]
    PROJECT_LIST_COLUMNS_TO_STRIP = ["reminders"]
    AVAILABLE_KEYS_FOR_PUT_AND_PATCH = ["name", "language", "contributors"]

    def get(self, project_id=None):
        self.get_project_by_id(project_id=project_id) if project_id else self.get_all_projects()

    def post(self):
        self.make_new_project()

    def delete(self, project_id=None):
        if project_id:
            self.delete_project_by_id(project_id=project_id)
        else:
            self.error(404)
                

    def put(self, project_id=None):
        if project_id:
            self.put_new_values_in_project_by_id(project_id=project_id)
        else:
            self.error(404)
                

    def patch(self, project_id=None):
        if project_id:
            self.patch_new_value_in_project_by_id(project_id=project_id)
        else:
            self.error(404)
                

    def get_all_projects(self):
        projects = Project.query().fetch()

        output_array = []

        for project in projects:
            temp_dict = project.to_dict()
            temp_dict = self.strip_key_value_pairs_from_dict(temp_dict, self.PROJECT_LIST_COLUMNS_TO_STRIP)
            output_array.append(temp_dict)

        self.response.write(json.dumps(output_array))

    def get_project_by_id(self, project_id):
        entity = get_ndb_entity_from_id(project_id)
        if not entity:
            self.error(404)
                
            return

        temp_dict = entity.to_dict()
        temp_dict = self.strip_key_value_pairs_from_dict(temp_dict, self.PROJECT_LIST_COLUMNS_TO_STRIP)

        self.response.write(json.dumps(temp_dict))

    def make_new_project(self):
        parsed_json = json.loads(self.request.body)

        if not self.NEW_PROJECT_REQUIRED_POST_VALUES <= set(parsed_json):
            self.error(404)
                
            return

        name = get_value_from_dict_or_none("name", parsed_json)
        language = get_value_from_dict_or_none("language", parsed_json)
        contributors = get_value_from_dict_or_none("contributors", parsed_json)

        new_project = Project(name=name, language=language, contributors=contributors)

        new_project.put()
        new_project.id = new_project.key.urlsafe()
        new_project.put()

        temp_dict = new_project.to_dict()
        temp_dict = self.strip_key_value_pairs_from_dict(temp_dict, self.PROJECT_LIST_COLUMNS_TO_STRIP)

        self.response.write(json.dumps(temp_dict))

    def delete_project_by_id(self, project_id):
        entity = get_ndb_entity_from_id(project_id)
        if not entity:
            self.error(404)
                
            return

        project_dict = entity.to_dict()

        reminder_ids = get_value_from_dict_or_none("reminders", project_dict)

        for reminder_id in reminder_ids:
            reminder_entity = get_ndb_entity_from_id(reminder_id)
            reminder_entity.key.delete()

        entity.key.delete()

    def put_new_values_in_project_by_id(self, project_id):
        project_entity = get_ndb_entity_from_id(project_id)
        if not project_entity:
            self.error(404)
                
            return

        parsed_json = json.loads(self.request.body)

        if not len(parsed_json):
            self.error(404)
                
            return

        temp_dict = get_dict_with_valid_key_value_pairs(parsed_json, self.AVAILABLE_KEYS_FOR_PUT_AND_PATCH)
        project_entity.populate(**temp_dict)
        project_entity.put()

    def patch_new_value_in_project_by_id(self, project_id):
        project_entity = get_ndb_entity_from_id(project_id)
        if not project_entity:
            self.error(404)
                
            return

        parsed_json = json.loads(self.request.body)

        if len(parsed_json) != 1:
            self.error(404)
                
            return

        key, _ = parsed_json.items()[0]

        if key in self.AVAILABLE_KEYS_FOR_PUT_AND_PATCH:
            project_entity.populate(**parsed_json)
            project_entity.put()
        else:
            self.error(404)
                
            return

    @staticmethod
    def strip_key_value_pairs_from_dict(input_dict, to_strip):
        for key in to_strip:
            if key in input_dict:
                del input_dict[key]
        return input_dict


class RestProjectReminderHandler(webapp2.RequestHandler):
    NEW_REMINDER_REQUIRED_POST_VALUES = ["title"]
    AVAILABLE_KEYS_FOR_PUT_AND_PATCH = ["title", "reminder_date", "users_to_notify", "text_content"]

    def get(self, project_id=None, reminder_id=None):
        if project_id and reminder_id:
            self.get_reminder_by_ids(project_id=project_id, reminder_id=reminder_id)
        elif project_id:
            self.get_all_reminders_for_project(project_id=project_id)
        else:
            self.error(404)
                

    def post(self, project_id=None):
        if project_id:
            self.make_new_project_reminder(project_id=project_id)
        else:
            self.error(404)
                

    def delete(self, project_id=None, reminder_id=None):
        if project_id and reminder_id:
            self.delete_reminder_by_ids(project_id=project_id, reminder_id=reminder_id)
        else:
            self.error(404)
                

    def put(self, project_id=None, reminder_id=None):
        if project_id and reminder_id:
            self.put_new_values_in_reminder_by_id(reminder_id=reminder_id)
        else:
            self.error(404)
                

    def patch(self, project_id=None, reminder_id=None):
        if project_id and reminder_id:
            self.patch_new_value_in_reminder_by_id(reminder_id=reminder_id)
        else:
            self.error(404)
                

    def get_all_reminders_for_project(self, project_id):
        project_entity = get_ndb_entity_from_id(project_id)  # type: Project
        if not project_entity:
            self.error(404)
                
            return

        project_dict = project_entity.to_dict()

        reminder_ids = get_value_from_dict_or_none("reminders", project_dict)

        output_reminder_array = []

        for reminder_id in reminder_ids:
            reminder_entity = get_ndb_entity_from_id(reminder_id)
            output_reminder_array.append(reminder_entity.to_dict())

        self.response.write(json.dumps(output_reminder_array))

    def get_reminder_by_ids(self, project_id, reminder_id):
        project_entity = get_ndb_entity_from_id(project_id)  # type: Project
        if not project_entity:
            self.error(404)
                
            return

        project_dict = project_entity.to_dict()

        reminder_ids = get_value_from_dict_or_none("reminders", project_dict)

        if reminder_id not in reminder_ids:
            self.error(404)
                
            return

        reminder_entity = get_ndb_entity_from_id(reminder_id)

        self.response.write(json.dumps(reminder_entity.to_dict()))

    def make_new_project_reminder(self, project_id):
        parsed_json = json.loads(self.request.body)

        # Check if the request has bare minimum number of required values
        if not self.NEW_REMINDER_REQUIRED_POST_VALUES <= set(parsed_json):
            self.error(404)
                
            return

        # Check if the project exists
        entity = get_ndb_entity_from_id(project_id)  # type: Project
        if not entity:
            self.error(404)
                
            return

        title = get_value_from_dict_or_none("title", parsed_json)
        reminder_date = get_value_from_dict_or_none("reminder_date", parsed_json)
        users_to_notify = get_value_from_dict_or_none("users_to_notify", parsed_json)
        text_content = get_value_from_dict_or_none("text_content", parsed_json)

        new_project_reminder = ProjectReminder(title=title, reminder_date=reminder_date, users_to_notify=users_to_notify, text_content=text_content)
        new_project_reminder.put()
        new_project_reminder.id = new_project_reminder.key.urlsafe()
        new_project_reminder.put()

        entity.reminders.append(new_project_reminder.id)
        entity.put()

        self.response.write(json.dumps(new_project_reminder.to_dict()))

    def delete_reminder_by_ids(self, project_id, reminder_id):
        project_entity = get_ndb_entity_from_id(project_id)  # type: Project
        if not project_entity:
            self.error(404)
                
            return

        project_dict = project_entity.to_dict()

        reminder_ids = get_value_from_dict_or_none("reminders", project_dict)

        if reminder_id not in reminder_ids:
            self.error(404)
                
            return

        reminder_entity = get_ndb_entity_from_id(reminder_id)

        reminder_entity.key.delete()

        reminder_id_index_in_project = project_entity.reminders.index(reminder_id)
        project_entity.reminders.pop(reminder_id_index_in_project)
        project_entity.put()

    def put_new_values_in_reminder_by_id(self, reminder_id):
        reminder_entity = get_ndb_entity_from_id(reminder_id)
        if not reminder_entity:
            self.error(404)
                
            return

        parsed_json = json.loads(self.request.body)

        if not len(parsed_json):
            self.error(404)
                
            return

        temp_dict = get_dict_with_valid_key_value_pairs(parsed_json, self.AVAILABLE_KEYS_FOR_PUT_AND_PATCH)
        reminder_entity.populate(**temp_dict)
        reminder_entity.put()

    def patch_new_value_in_reminder_by_id(self, reminder_id):
        reminder_entity = get_ndb_entity_from_id(reminder_id)
        if not reminder_entity:
            self.error(404)
                
            return

        parsed_json = json.loads(self.request.body)

        if len(parsed_json) != 1:
            self.error(404)
                
            return

        key, _ = parsed_json.items()[0]

        if key in self.AVAILABLE_KEYS_FOR_PUT_AND_PATCH:
            reminder_entity.populate(**parsed_json)
            reminder_entity.put()
        else:
            self.error(404)
                
            return


def get_dict_with_valid_key_value_pairs(input_dict, valid_values):
    output_dict = {}

    for dict_key in input_dict:
        if dict_key in valid_values:
            output_dict[dict_key] = input_dict[dict_key]

    return output_dict


def get_value_from_dict_or_none(value, dict_to_check):
    return dict_to_check[value] if value in dict_to_check else None


def get_ndb_entity_from_id(id_to_find):
    key = ndb.Key(urlsafe=id_to_find)
    return key.get()
