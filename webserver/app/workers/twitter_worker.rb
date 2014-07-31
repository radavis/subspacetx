require 'sidekiq'
require 'sidetiq'
require 'twitter'
require 'sinatra/activerecord'
require_relative '../models/tweet'

class TwitterWorker
  include Sidekiq::Worker
  include Sidetiq::Schedulable

  recurrence { minutely(5) }

  def perform()
    client = Twitter::REST::Client.new do |config|
      config.consumer_key = ENV['TWITTER_CONSUMER_KEY']
      config.consumer_secret = ENV['TWITTER_CONSUMER_SECRET']
    end

    results = client.search("#subspacetx")
    results.each do |result|
      tweet = Tweet.find_or_initialize_by(tweet_id: result.id.to_s)
      tweet.username = result.user.screen_name
      tweet.message = result.text
      tweet.profile_image = result.user.profile_image_url.to_s
      tweet.save
    end
  end
end
