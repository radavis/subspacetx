require 'sinatra'
require 'sinatra/activerecord'
require 'json'
#require 'pry'

Dir[File.join(File.dirname(__FILE__), 'app', '**', '*.rb')].each do |file|
  require file
end

configure do
  set :views, 'app/views'
end

get '/' do
  redirect to '/tweets'
end

get '/tweets' do
  @tweets = Tweet.order(created_at: :desc).limit(50)
  erb :tweets
end

get '/tweets.json' do
  content_type :json
  Tweet.order(created_at: :desc).limit(10).to_json
end

get '/tweets.txt' do
  content_type :text
  Tweet.last.to_text
end
