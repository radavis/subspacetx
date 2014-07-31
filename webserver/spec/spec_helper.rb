require 'pry'
require 'rspec'
require 'capybara/rspec'
require 'factory_girl'
require 'valid_attribute'
require 'shoulda/matchers'

require_relative '../app.rb'

set :environment, :test
set :database, :test

Capybara.app = Sinatra::Application

FactoryGirl.definition_file_paths = %w{ ./spec/factories }
FactoryGirl.find_definitions
