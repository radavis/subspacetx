class Tweet < ActiveRecord::Base
  USERNAME_LENGTH = 15
  MESSAGE_LENGTH = 140

  validates_presence_of :username
  validates_length_of :username, { minimum: 1, maximum: USERNAME_LENGTH }

  validates_presence_of :message
  validates_length_of :message, { minimum: 1, maximum: MESSAGE_LENGTH }

  validates_uniqueness_of :tweet_id

  def as_json(options={})
    { username: "@#{username}", message: message }
  end

  def to_text
    "@#{username_padded}\n#{message_padded}"
  end

  def username_padded
    spaces = ' ' * (USERNAME_LENGTH - username.length)
    "#{username}#{spaces}"
  end

  def message_padded
    spaces = ' ' * (MESSAGE_LENGTH - message.length)
    "#{message}#{spaces}"
  end
end
