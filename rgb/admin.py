from django.contrib import admin

from .models import Command, Routine

class CommandInline(admin.TabularInline):
    model = Command
    extra = 3

class RoutineAdmin(admin.ModelAdmin):
    inlines = [CommandInline]
    list_display = ('name', 'category')

admin.site.register(Routine, RoutineAdmin)

