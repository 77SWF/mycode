# Generated by Django 2.0.13 on 2020-04-25 05:16

from django.db import migrations


class Migration(migrations.Migration):

    dependencies = [
        ('tasks_square', '0001_initial'),
    ]

    operations = [
        migrations.DeleteModel(
            name='tasks_display_order',
        ),
    ]