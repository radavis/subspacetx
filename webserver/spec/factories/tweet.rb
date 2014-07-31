FactoryGirl.define do
  factory :tweet do
    sequence(:username) { |n| "borg_#{n}" }
    message 'You will be assimilated'
    sequence(:tweet_id) { |n| n }
    profile_image { 'http://www.twitter.com/borg_image.jpg' }
  end
end
